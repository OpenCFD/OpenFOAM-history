/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __CSI_BASE_H__
#define __CSI_BASE_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace CSI
{

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> OID;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > OID_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > OID_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OID;

typedef SequenceTmpl< OID,MICO_TID_DEF> OIDList;
typedef TSeqVar< SequenceTmpl< OID,MICO_TID_DEF> > OIDList_var;
typedef TSeqOut< SequenceTmpl< OID,MICO_TID_DEF> > OIDList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OIDList;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSI::OID,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSI::OID,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSI::OID,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSI::OID,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq__seq_octet;

#endif
