#include <CORBA.h>
#include "address.cc"
#include "any.cc"
#include "basic_seq.cc"
#include "buffer.cc"
#include "codec.cc"
#include "context.cc"
#include "dii.cc"
#include "dsi.cc"
#include "iop.cc"
#include "ior.cc"
#include "object.cc"
#include "value.cc"
#include "valuetype_impl.cc"
#include "orb.cc"
#include "string.cc"
#include "except.cc"
#include "transport.cc"
#include "transport/tcp.cc"
#include "transport/udp.cc"
#include "transport/unix.cc"
#include "dispatch.cc"
#include "typecode.cc"
#include "util.cc"
#include "fixed.cc"
#include "codeset.cc"
#include "queue.cc"
#include "static.cc"
#include "intercept.cc"
#include "os-unix.cc"
#include "ssl.cc"
#include "current.cc"
#include "valuetype.cc"
#include "policy2.cc"
#include "dynany_impl.cc"
#include "policy_impl.cc"
#include "service_info.cc"
#include "ioptypes.cc"
#include "tckind.cc"
#include "orb_excepts.cc"
#include "policy.cc"
#include "poa.cc"
#include "poa_base.cc"
#include "poa_impl.cc"
#include "dynany.cc"
#include "ssliop.cc"

#ifdef USE_WIRELESS
#include "watm.cc"
#endif // USE_WIRELESS

#ifdef HAVE_BLUETOOTH
#include "ltp_address.cc"
#include "ltp_ior.cc"
#include "transport/ltp.cc"
#endif // HAVE_BLUETOOTH

// address.cc and others define MICO_CONF_NO_POA
#undef MICO_CONF_NO_POA
#include "ir_base.cc"
#include "ir.cc"
#include "imr.cc"

#include "mtdebug.cc"
#include "fast_array.cc"
#include "pi.cc"
#include "pi_impl.cc"
#include "typecode_seq.cc"

#include "timebase.cc"

// needed by both CSIv2 and CSL2
#if (defined(USE_CSL2)) || (defined(USE_CSIV2))
#include "security/csi_base.cc"
#endif // USE_CSL2 or USE_CSIV2

#ifdef USE_CSIV2
#include "security/csi.cc"
#include "security/csiiop.cc"
#include "security/gssup.cc"
#include "security/csiv2.cc"
#include "security/csiv2_impl.cc"
#endif // USE_CSIV2

#ifdef USE_CSL2
#include "security/security.cc"
#include "security/securitylevel1.cc"
#include "security/securitylevel1_impl.cc"
#include "security/securitylevel2.cc"
#include "security/SecurityAdmin.cc"
#include "security/SecurityAdmin_impl.cc"
#include "security/AuditClientIntercept.cc"
#include "security/AuditOptions.cc"
#include "security/AuditServIntercept.cc"
#include "security/ODMConfig.cc"
#include "security/odm.cc"
#include "security/DomainManager.cc"
//
// Files which can't be included (from various reasons)
//
//#include "security/securitylevel2_impl.cc"
//#include "security/audit_impl.cc"
//#include "security/odm_impl.cc"
//#include "security/AccessConfig.cc"
//#include "security/DomainManager_impl.cc"
//#include "security/RightsConfig.cc"
//#include "security/AuditConfig.cc"
#endif // USE_CSL2

#ifdef HAVE_THREADS
#include "os-thread.cc"
#include "operation.cc"
#include "message.cc"
#include "mt_dispatcher.cc"
#include "mt_manager.cc"
#endif // HAVE_THREADS
