// -*- c++ -*-
//
// $Id: pdatatype.h,v 1.4 2003/04/15 01:31:33 adharurk Exp $
//
// Copyright (c) 2001-2003 The Trustees of Indiana University.  
//                         All rights reserved.
// Copyright (c) 1998-2001 University of Notre Dame. 
//                         All rights reserved.
// Copyright (c) 1994-1998 The Ohio State University.  
//                         All rights reserved.
// 
// This file is part of the LAM/MPI software package.  For license
// information, see the LICENSE file in the top level directory of the
// LAM/MPI source distribution.
// 
// $HEADER$
//


class Datatype {
public:
  // construction / destruction
  inline Datatype() : mpi_datatype(MPI_DATATYPE_NULL) { }
  inline virtual ~Datatype() {}
  // inter-language operability
  inline Datatype(const MPI_Datatype &i) : mpi_datatype(i) { }

  // copy / assignment
  Datatype(const Datatype& dt) : mpi_datatype(dt.mpi_datatype) { }

  Datatype& operator=(const Datatype& dt) {
    mpi_datatype = dt.mpi_datatype; return *this;
  }
  
  // comparison
  bool operator==(const Datatype &a) const {
    return (bool) (mpi_datatype == a.mpi_datatype);
  }

  inline bool operator!= (const Datatype &a) const 
    { return (bool) !(*this == a); }

  // inter-language operability
  inline Datatype& operator= (const MPI_Datatype &i) 
    { mpi_datatype = i; return *this; }

  inline operator MPI_Datatype () const { return mpi_datatype; }

  inline const MPI_Datatype& mpi() const { return mpi_datatype; }


  //
  // User Defined Functions
  //
  typedef int Copy_attr_function(const Datatype& oldtype,
				 int type_keyval,
				 void* extra_state,
				 const void* attribute_val_in,
				 void* attribute_val_out,
				 bool& flag);

  typedef int Delete_attr_function(Datatype& type, int type_keyval,
                                   void* attribute_val, void* extra_state);


  //
  // Point-to-Point Communication
  //

  inline virtual Datatype Create_contiguous(int count) const;

  virtual Datatype Create_vector(int count, int blocklength,
					int stride) const;

  virtual Datatype Create_indexed(int count,
				       const int array_of_blocklengths[], 
				       const int array_of_displacements[]) const;
  
  static Datatype Create_struct(int count, const int array_of_blocklengths[],
				     const Aint array_of_displacements[],
				     const Datatype array_if_types[]);
  

  virtual Datatype Create_hindexed(int count, const int array_of_blocklengths[],
					const Aint array_of_displacements[]) const;

  virtual Datatype Create_hvector(int count, int blocklength,
				       Aint stride) const;

  virtual int Get_size() const;

  virtual void Get_extent(Aint& lb, Aint& extent) const;

  virtual void Commit();

  virtual void Free();

  //JGS I did not make these inline becuase of the dependency
  //on the Comm conversion function
  virtual void Pack(const void* inbuf, int incount, void *outbuf, 
		    int outsize, int& position, const Comm &comm) const;
  virtual void Unpack(const void* inbuf, int insize, void *outbuf,
		      int outcount, int& position, const Comm& comm) const;
  virtual int Pack_size(int incount, const Comm& comm) const;


  //
  // Miscellany
  //
  virtual Datatype Create_subarray(int ndims, const int array_of_sizes[],
                                   const int array_of_subsizes[],
                                   const int array_of_starts[], int order)
    const;



  //
  // External Intefaces
  //

  virtual Datatype Dup() const;
  
  static int Create_keyval(Copy_attr_function* type_copy_attr_fn,
                           Delete_attr_function* type_delete_attr_fn,
                           void* extra_state);

  virtual void Delete_attr(int type_keyval);

  static void Free_keyval(int& type_keyval);

  virtual bool Get_attr(int type_keyval, void* attribute_val) const;

  virtual void Get_contents(int max_integers, int max_addresses,
                            int max_datatypes, int array_of_integers[],
                            Aint array_of_addresses[],
                            Datatype array_of_datatypes[]) const;

  virtual void Get_envelope(int& num_integers, int& num_addresses,
                            int& num_datatypes, int& combiner) const;

  virtual void Get_name(char* type_name, int& resultlen) const;

  virtual void Set_attr(int type_keyval, const void* attribute_val);

  virtual void Set_name(const char* type_name);


protected:
  MPI_Datatype mpi_datatype;

};

