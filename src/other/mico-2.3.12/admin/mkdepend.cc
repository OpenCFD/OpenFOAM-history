/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-2005 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#include <mico/config.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <string>
#include <vector>
#include <set>

using namespace std;

typedef vector<string> svec;
typedef set<string, less<string> > sset;

void
print (string file, const sset &deps)
{
    long pos = file.find_last_of ('.');
    if (pos >= 0)
	file = file.substr (0, pos);
    string out = file + ".o " + file + ".pic.o" + ":"; 
    for (sset::const_iterator i = deps.begin(); i != deps.end(); ++i) {
        if (out.length() > 70) {
            cout << out << endl;
            out = file + ".o " + file + ".pic.o" + ":"; 
        }
        out += " ";
        out += *i;
    }
    cout << out << endl;
}

bool
cc_file(const string& file)
{
    unsigned long len = file.length();
    // .c file
    if (len > 2 && file[len-1] == 'c' && file[len-2] == '.')
	return true;
    // .C file
    if (len > 2 && file[len-1] == 'C' && file[len-2] == '.')
	return true;
    // .cc file
    if (len > 3 && file[len-1] == 'c' && file[len-2] == 'c'
	&& file[len-3] == '.')
	return true;
    // .cxx file
    if (len > 4 && file[len-1] == 'x' && file[len-2] == 'x'
	&& file[len-3] == 'c' && file[len-4] == '.')
	return true;
    // .cpp file
    if (len > 4 && file[len-1] == 'p' && file[len-2] == 'p'
	&& file[len-3] == 'c' && file[len-4] == '.')
	return true;
    return false;
}

void
mkdep (const string &file, const svec &incdirs)
{
    char rawline[10000];
    svec stack;
    sset deps;
    // check if we operate on 'real' C/C++ file
    // i.e. this aims to exclude all directories from mkdep generation
    // process
    if (!cc_file(file))
  	return;
    stack.push_back (file);
    while (!stack.empty()) {
        string f = stack.back();
        stack.pop_back ();

        ifstream inp (f.c_str());
        if (!inp) {
            // cerr << f << ": cannot open, skipped." << endl;
            continue;
        }
        deps.insert (f);

        while (inp.getline (rawline, sizeof(rawline)).good()) {
            string line = rawline;
	    bool real_path = false;
            if (line.length() == 0 || line[0] != '#')
                continue;
            if ((long)line.find ("include") >= 0) {
                long pos1, pos2 = 0;
                pos1 = line.find ('<');
                if (pos1 >= 0)
                    pos2 = line.find ('>', ++pos1);
                else if ((pos1 = line.find ('"')) >= 0) {
                    pos2 = line.find ('"', ++pos1);
		    real_path = true;
		}
                if (pos1 >= 0 && pos2 >= 0) {
                    string incfile = line.substr (pos1, pos2-pos1);
                    for (svec::size_type i = 0; i < incdirs.size(); ++i) {
                        string t = incdirs[i] + "/" + incfile;
                        if (!access (t.c_str(), R_OK)) {
                            if (deps.find (t) == deps.end())
                                stack.push_back (t);
                            break;
                        }
                    }
		    if (real_path) {
			string t = incfile;
                        if (!access (t.c_str(), R_OK)) {
                            if (deps.find (t) == deps.end())
                                stack.push_back (t);
			}
		    }
                }
            }
        }
    }
    if (deps.size() > 0)
	print (file, deps);
}

/*************** Micro getopt() *********************************************/
#define OPTION(c,v)     (_O&2&&**v?*(*v)++:!c||_O&4?0:(!(_O&1)&& \
                                (--c,++v),_O=4,c&&**v=='-'&&v[0][1]?*++*v=='-'\
                                &&!v[0][1]?(--c,++v,0):(_O=2,*(*v)++):0))
#define OPTARG(c,v)     (_O&2?**v||(++v,--c)?(_O=1,--c,*v++): \
                                (_O=4,(char*)0):(char*)0)
#define OPTONLYARG(c,v) (_O&2&&**v?(_O=1,--c,*v++):(char*)0)
#define ARG(c,v)        (c?(--c,*v++):(char*)0)

static int _O = 0;              /* Internal state */
/*************** Micro getopt() *********************************************/

int
main (int argc, char *argv[])
{
    svec incdirs;

    int c;
    char *arg;
    string s;
    while ((c = OPTION (argc, argv))) {
	switch (c) {
	case 'I':
	    arg = OPTARG (argc, argv);
	    assert (arg);
            // hack for cygnus CDK. convert back E:/... to //E/...
            s = arg;
            if (s.length() >= 2 && s[1] == ':')
                s = string("//") + s[0] + s.substr (2);
            incdirs.push_back (s);
            break;
	case 'D':
	    arg = OPTARG (argc, argv);
	    assert (arg);
	    break;
	case 'f':
	case 'm':
	    OPTARG (argc, argv);
	    break;
	case 'g':
	    break;
	case 'O':
	case '0':
	case '1':
	case '2':
	    break;
	default:
	    // cerr << "unknown option: -" << (char)c << endl;
	    break;
	}
    }
    while ((arg = ARG (argc, argv))) {
	mkdep (arg, incdirs);
    }
    return 0;
}
