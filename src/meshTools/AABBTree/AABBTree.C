/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "AABBTree.H"
#include "meshTools.H"
#include "OFstream.H"

template<class Type>
Foam::scalar Foam::AABBTree<Type>::tolerance_ = 1e-4;

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class Type>
void Foam::AABBTree<Type>::writeOBJ
(
    const bool writeLinesOnly,
    const treeBoundBox& bb,
    label& vertI,
    Ostream& os
) const
{
    const pointField pts(bb.points());
    forAll(pts, i)
    {
        meshTools::writeOBJ(os, pts[i]);
    }

    if (writeLinesOnly)
    {
        forAll(bb.edges, i)
        {
            const edge& e = bb.edges[i];
            os  << "l " << e[0] + vertI + 1 << ' ' << e[1] + vertI + 1 << nl;
        }
    }
    else
    {
        forAll(bb.faces, i)
        {
            const face& f = bb.faces[i];

            os  << 'f';
            forAll(f, fp)
            {
                os  << ' ' << f[fp] + vertI + 1;
            }
            os  << nl;
        }
    }

    vertI += pts.size();
}


template<class Type>
void Foam::AABBTree<Type>::writeOBJ
(
    const bool leavesOnly,
    const bool writeLinesOnly,
    const treeBoundBox& bb,
    const label nodeI,
    const List<Pair<treeBoundBox> >& bbs,
    const List<Pair<label> >& nodes,
    label& vertI,
    Ostream& os
) const
{
    if (!leavesOnly || nodeI < 0)
    {
        writeOBJ(writeLinesOnly, bb, vertI, os);
    }

    // recurse to find leaves
    if (nodeI >= 0)
    {
        writeOBJ
        (
            leavesOnly,
            writeLinesOnly,
            bbs[nodeI].first(),
            nodes[nodeI].first(),
            bbs,
            nodes,
            vertI,
            os
        );
        writeOBJ
        (
            leavesOnly,
            writeLinesOnly,
            bbs[nodeI].second(),
            nodes[nodeI].second(),
            bbs,
            nodes,
            vertI,
            os
        );
    }
}


template<class Type>
void Foam::AABBTree<Type>::createBoxes
(
    const label level,
    const List<Type>& objects,
    const pointField& points,
    const DynamicList<label>& objectIDs,
    const treeBoundBox& bb,
    const label nodeI,

    DynamicList<Pair<treeBoundBox> >& bbs,
    DynamicList<labelPair>& nodes,
    DynamicList<labelList>& addressing
) const
{
    const vector span = bb.span();

    // determine which direction to divide the box
    direction maxDir = 0;
    scalar maxSpan = span[maxDir];
    for (label dirI = 1; dirI < 3; dirI++)
    {
        if (span[dirI] > maxSpan)
        {
            maxSpan = span[dirI];
            maxDir = dirI;
        }
    }


    // allocate the points into the min bin
    DynamicList<label> minBinObjectIDs(objectIDs.size());
    treeBoundBox minBb(point::max, point::min);
    {
        const scalar divide = bb.min()[maxDir] + (0.5 + tolerance_)*maxSpan;

        boolList markedPoints(points.size(), false);
        forAll(points, pointI)
        {
            const point& pt = points[pointI];
            if (pt[maxDir] < divide)
            {
                markedPoints[pointI] = true;
            }
        }

        forAll(objectIDs, i)
        {
            const label objI = objectIDs[i];
            const Type& obj = objects[objI];

            bool addObject = false;
            forAll(obj, pI)
            {
                const label pointI = obj[pI];
                if (markedPoints[pointI])
                {
                    addObject = true;
                    break;
                }
            }

            if (addObject)
            {
                minBinObjectIDs.append(objI);
                const boundBox objBb(points, obj, false);
                minBb.min() = min(minBb.min(), objBb.min());
                minBb.max() = max(minBb.max(), objBb.max());
            }
        }

        // inflate box in case geometry reduces to 2-D
        if (minBinObjectIDs.size())
        {
            minBb.inflate(0.01);
        }
    }

    // allocate the points into the max bin
    DynamicList<label> maxBinObjectIDs(objectIDs.size());
    treeBoundBox maxBb(point::max, point::min);
    {
        const scalar divide = bb.min()[maxDir] + (0.5 - tolerance_)*maxSpan;

        boolList markedPoints(points.size(), false);
        forAll(points, pointI)
        {
            const point& pt = points[pointI];
            if (pt[maxDir] > divide)
            {
                markedPoints[pointI] = true;
            }
        }

        forAll(objectIDs, i)
        {
            const label objI = objectIDs[i];
            const Type& obj = objects[objI];

            bool addObject = false;
            forAll(obj, pI)
            {
                const label pointI = obj[pI];
                if (markedPoints[pointI])
                {
                    addObject = true;
                    break;
                }
            }

            if (addObject)
            {
                maxBinObjectIDs.append(objI);
                const boundBox objBb(points, obj, false);
                maxBb.min() = min(maxBb.min(), objBb.min());
                maxBb.max() = max(maxBb.max(), objBb.max());
            }
        }

        // inflate box in case geometry reduces to 2-D
        if (maxBinObjectIDs.size())
        {
            maxBb.inflate(0.01);
        }
    }

    minBinObjectIDs.shrink();
    maxBinObjectIDs.shrink();

    label minI;
    if (minBinObjectIDs.size() > minLeafSize_ && level < maxLevel_)
    {
        // new leaf
        minI = nodes.size();
        nodes.append(labelPair(-1, -1));
    }
    else
    {
        // update existing leaf
        minI = -addressing.size() - 1;
        addressing.append(minBinObjectIDs);
    }

    label maxI;
    if (maxBinObjectIDs.size() > minLeafSize_ && level < maxLevel_)
    {
        // new leaf
        maxI = nodes.size();
        nodes.append(labelPair(-1, -1));
    }
    else
    {
        // update existing leaf
        maxI = -addressing.size() - 1;
        addressing.append(maxBinObjectIDs);
    }

    nodes(nodeI) = labelPair(minI, maxI);
    bbs(nodeI) = Pair<treeBoundBox>(minBb, maxBb);

    // recurse
    if (minI >= 0)
    {
        createBoxes
        (
            level + 1,
            objects,
            points,
            minBinObjectIDs,
            minBb,
            minI,
            bbs,
            nodes,
            addressing
        );
    }
    if (maxI >= 0)
    {
        createBoxes
        (
            level + 1,
            objects,
            points,
            maxBinObjectIDs,
            maxBb,
            maxI,
            bbs,
            nodes,
            addressing
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::AABBTree<Type>::AABBTree()
:
    maxLevel_(0),
    minLeafSize_(0),
    boundBoxes_(),
    addressing_()
{}


template<class Type>
Foam::AABBTree<Type>::AABBTree
(
    const List<Type>& objects,
    const pointField& points,
    const label maxLevel,
    const label minLeafSize
)
:
    maxLevel_(maxLevel),
    minLeafSize_(minLeafSize),
    boundBoxes_(),
    addressing_()
{
    DynamicList<Pair<treeBoundBox> > bbs(maxLevel);
    DynamicList<labelPair> nodes(maxLevel);
    DynamicList<labelList> addr(maxLevel);

    nodes.append(labelPair(-1, -1));
    treeBoundBox topBb(points);
    topBb.inflate(0.01);

    DynamicList<label> objectIDs(identity(objects.size()));

    createBoxes
    (
        0,          // starting at top level
        objects,
        points,
        objectIDs,
        topBb,
        0,          // starting node

        bbs,
        nodes,
        addr
    );


//    {
//        OFstream os("tree.obj");
//        label vertI = 0;
//        writeOBJ
//        (
//            true,       // leavesOnly
//            false,      // writeLinesOnly
//
//            topBb,
//            0,
//            bbs,
//            nodes,
//            vertI,
//            os
//        );
//    }


    // transfer flattened tree to persistent storage
    DynamicList<treeBoundBox> boundBoxes(2*bbs.size());
    DynamicList<labelList> addressing(2*addr.size());
    forAll(nodes, nodeI)
    {
        if (nodes[nodeI].first() < 0)
        {
            boundBoxes.append(bbs[nodeI].first());
            addressing.append(addr[nodeI + 1]);
        }
        if (nodes[nodeI].second() < 0)
        {
            boundBoxes.append(bbs[nodeI].second());
            addressing.append(addr[nodeI + 1]);
        }
    }

    boundBoxes_.transfer(boundBoxes);
    addressing_.transfer(addressing);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const Foam::List<Foam::treeBoundBox>& Foam::AABBTree<Type>::boundBoxes() const
{
    return boundBoxes_;
}


template<class Type>
const Foam::List<Foam::labelList>& Foam::AABBTree<Type>::addressing() const
{
    return addressing_;
}


template<class Type>
bool Foam::AABBTree<Type>::pointInside(const point& pt) const
{
    forAll(boundBoxes_, i)
    {
        const treeBoundBox& bb = boundBoxes_[i];

        if (bb.contains(pt))
        {
            return true;
        }
    }

    return false;
}


template<class Type>
bool Foam::AABBTree<Type>::overlaps(const boundBox& bbIn) const
{
    forAll(boundBoxes_, i)
    {
        const treeBoundBox& bb = boundBoxes_[i];

        if (bb.overlaps(bbIn))
        {
            return true;
        }
    }

    return false;
}


// * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //

template<class Type>
Foam::Ostream& Foam::operator<<(Ostream& os, const AABBTree<Type>& tree)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << tree.maxLevel_ << token::SPACE
            << tree.minLeafSize_ << token::SPACE
            << tree.boundBoxes_ << token::SPACE
            << tree.addressing_ << token::SPACE;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&tree.maxLevel_),
            sizeof(tree.maxLevel_)
          + sizeof(tree.minLeafSize_)
        );
        os  << tree.boundBoxes_
            << tree.addressing_;
    }

    os.check("Ostream& operator<<(Ostream&, const AABBTree<Type>&)");
    return os;
}


template<class Type>
Foam::Istream& Foam::operator>>(Istream& is, AABBTree<Type>& tree)
{
    if (is.format() == IOstream::ASCII)
    {
        is  >> tree.maxLevel_
            >> tree.minLeafSize_
            >> tree.boundBoxes_
            >> tree.addressing_;
    }
    else
    {
        is.read
        (
            reinterpret_cast<char*>(&tree.maxLevel_),
            sizeof(tree.maxLevel_)
          + sizeof(tree.minLeafSize_)
        );
        is  >> tree.boundBoxes_
            >> tree.addressing_;
    }

    is.check("Istream& operator>>(Istream&, AABBTree<Type>&)");
    return is;
}


// ************************************************************************* //
