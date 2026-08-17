/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_encoder.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_encoder.h"
#include <stddef.h>

int m3ap_encode_pdu(M3AP_M3AP_PDU_t *pdu, uint8_t **buffer, uint32_t *len) { return 0; }

