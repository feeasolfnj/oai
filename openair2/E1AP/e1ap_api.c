/* Auto-generated protocol stub for RISC-V port.
 * Replaces e1ap_api.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* e1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "e1ap_common.h"
#include "e1ap_api.h"
#include <stddef.h>

void cuup_init_n3(instance_t instance) { }

void process_e1_bearer_context_setup_req(instance_t, e1ap_bearer_setup_req_t *const req) { }

void CUUP_process_bearer_context_mod_req(instance_t, e1ap_bearer_setup_req_t *const req) { }

void CUUP_process_bearer_release_command(instance_t, e1ap_bearer_release_cmd_t *const cmd) { }

