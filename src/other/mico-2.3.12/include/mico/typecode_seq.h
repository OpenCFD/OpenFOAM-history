/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __TYPECODE_SEQ_H__
#define __TYPECODE_SEQ_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace CORBA
{

typedef SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> TypeCodeSeq;
typedef TSeqVar< SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> > TypeCodeSeq_var;
typedef TSeqOut< SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> > TypeCodeSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TypeCodeSeq;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_TypeCode;

#endif
