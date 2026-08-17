/* Auto-generated protocol stub for RISC-V port.
 * Replaces m2ap_decoder.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m2ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m2ap_common.h"
#include "m2ap_eNB_defs.h"
#include "m2ap_MCE_defs.h"
#include "m2ap_decoder.h"
#include <stddef.h>

int m2ap_decode_pdu(M2AP_M2AP_PDU_t *pdu, const uint8_t *const buffer, uint32_t length) { return 0; }

