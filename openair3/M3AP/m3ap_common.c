/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_common.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_common.h"
#include <stddef.h>

ssize_t m3ap_generate_successfull_outcome( uint8_t               **buffer, uint32_t               *length, M3AP_ProcedureCode_t         procedureCode, M3AP_Criticality_t           criticality, asn_TYPE_descriptor_t  *td, void                   *sptr) { return 0; }

ssize_t m3ap_generate_initiating_message( uint8_t               **buffer, uint32_t               *length, M3AP_ProcedureCode_t    procedureCode, M3AP_Criticality_t      criticality, asn_TYPE_descriptor_t  *td, void                   *sptr) { return 0; }

ssize_t m3ap_generate_unsuccessfull_outcome( uint8_t               **buffer, uint32_t               *length, M3AP_ProcedureCode_t         procedureCode, M3AP_Criticality_t           criticality, asn_TYPE_descriptor_t  *td, void                   *sptr) { return 0; }

void m3ap_handle_criticality(M3AP_Criticality_t criticality) { }

