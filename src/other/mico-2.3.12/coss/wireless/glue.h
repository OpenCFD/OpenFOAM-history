// -*- c++ -*-
/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *  Copyright (C) 2001 Jaakko Kangasharju
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
 */

#ifndef __mico_glue_h__
#define __mico_glue_h__

/*
 * The glue classes provide an interface between the GTP-handling
 * proxy and the GIOP-handling bridge.  The interface is intentionally
 * very generic so that it could be used e.g. also for connecting two
 * bridges into a generic GIOP bridge.  However, this would require
 * agreement on the responsibility on generating various ids, which
 * currently is handled solely by the proxy, so currently this is
 * limited solely to the proxy-bridge connection.
 */
namespace WATMGlue {

/*
 * The IdGenerator class is for generating a sequence of unique
 * integer identifiers.  The constructor's arguments are the initial
 * value of the counter and the increment between two values.
 * new_id() gives the next id in the sequence (which wraps around
 * after reaching the type's maximum value) and release_id() puts its
 * argument back into the pool of available ids.  INVALID_ID is never
 * generated.
 */
class IdGenerator {
    CORBA::ULong current;
    CORBA::ULong step;
    std::set <CORBA::ULong> used_ids;
public:
    static const CORBA::ULong INVALID_ID;

    IdGenerator (CORBA::ULong init = 0, CORBA::ULong step = 1);
    CORBA::ULong new_id ();
    void release_id (CORBA::ULong);
};

/*
 * TargetAddress is basically the same as GIOP::TargetAddress in CORBA
 * with methods for using it.  The constructor leaves the new object
 * in a useless state (kind == Nil) and the only way to fill it is by
 * reading a CDR-encoded value with decode().  encode() is for
 * CDR-encoding a filled address (not of kind Nil).
 */
struct TargetAddress {
    enum { Key, Profile, Reference, Nil };
    CORBA::Short kind;
    union {
	CORBA::OctetSeq *key;
	CORBA::IORProfile *profile;
	struct {
	    CORBA::ULong index;
	    CORBA::IOR *ior;
	} reference;
    } u;

    TargetAddress ();
    ~TargetAddress ();

    void encode (CORBA::DataEncoder &) const;
    CORBA::Boolean decode (CORBA::DataDecoder &);

    void dump (std::ostream &) const;
private:
    void free ();
};

/*
 * BridgeRequest is the actual glue that is used for communicating
 * between the two parts of the system.  Each part is registered as a
 * BridgeRequestCallback in the other.  The events are modelled after
 * the GTP protocol's view of invocations (first Open, then Invokes,
 * then Close).  Error event is needed because of the asynchronous
 * nature of the GTP implementation: success of callback() cannot be
 * known when returning from a call.
 *
 * The valid members of a BridgeRequest depend on the Event argument
 * of callback().  For each Event they are:
 * Open: target
 * Invoke: conn_id, giop_data
 * Close: conn_id
 * Error: conn_id, code
 * The data_type and req_id elements are a hack for doing proper
 * location forwarding.  They are set when decoding and can (possibly)
 * be checked afterwards.
 */
struct BridgeRequest {
    CORBA::ULong conn_id;
    TargetAddress *target;
    CORBA::Buffer *giop_data;
    GIOP::MsgType data_type;
    CORBA::ULong req_id;

    CORBA::ULong code;

    BridgeRequest ();
    ~BridgeRequest ();

    void dump (std::ostream &) const;
};

/*
 * BridgeRequestCallback is inspired by MICO's general structure in
 * which callback interfaces feature prominently.  callback() fills in
 * various parts of its BridgeRequest argument.  For Open, conn_id is
 * filled with a unique id that is later used to refer to the opened
 * connection.  For all non-Error events, code is filled with an
 * ErrorCode that indicates whether the operation was successful
 * (NO_ERR==success, others==failure).
 */
struct BridgeRequestCallback {
    enum Event { Open, Close, Invoke, Error };
    enum ErrorCode { NO_ERR, INVALID_ID, TIMEOUT, COMM_FAILURE, RESOURCE,
		     FORWARD, UNKNOWN };
    virtual CORBA::Boolean callback (BridgeRequest *, Event) = 0;
    virtual ~BridgeRequestCallback () {}

    virtual CORBA::Boolean active (CORBA::ULong conn_id) {
	return TRUE;
    }
};

}

#endif
