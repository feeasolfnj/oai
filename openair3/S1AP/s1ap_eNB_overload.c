/* Auto-generated protocol stub for RISC-V port.
 * Replaces s1ap_eNB_overload.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* s1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "s1ap_common.h"
#include "intertask_interface.h"
#include "s1ap_eNB_defs.h"
#include "s1ap_eNB_overload.h"
#include <stddef.h>

int s1ap_eNB_handle_overload_start(uint32_t         assoc_id, uint32_t         stream, S1AP_S1AP_PDU_t *pdu) { return 0; }

int s1ap_eNB_handle_overload_stop(uint32_t         assoc_id, uint32_t         stream, S1AP_S1AP_PDU_t *pdu) { return 0; }

