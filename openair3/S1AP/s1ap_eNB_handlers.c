/* Auto-generated protocol stub for RISC-V port.
 * Replaces s1ap_eNB_handlers.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* s1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "s1ap_common.h"
#include "intertask_interface.h"
#include "s1ap_eNB_defs.h"
#include "s1ap_eNB_handlers.h"
#include <stddef.h>

void s1ap_handle_s1_setup_message(s1ap_eNB_mme_data_t *mme_desc_p, int sctp_shutdown) { }

int s1ap_eNB_handle_message(uint32_t assoc_id, int32_t stream, const uint8_t * const data, const uint32_t data_length) { return 0; }

