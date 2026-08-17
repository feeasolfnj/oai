/* Auto-generated protocol stub for RISC-V port.
 * Replaces s1ap_eNB_nas_procedures.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* s1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "s1ap_common.h"
#include "intertask_interface.h"
#include "s1ap_eNB_defs.h"
#include "s1ap_eNB_nas_procedures.h"
#include <stddef.h>

int s1ap_eNB_handle_nas_downlink( uint32_t         assoc_id, uint32_t         stream, S1AP_S1AP_PDU_t *pdu) { return 0; }

int s1ap_eNB_nas_uplink(instance_t instance, s1ap_uplink_nas_t *s1ap_uplink_nas_p) { return 0; }

int s1ap_eNB_nas_non_delivery_ind(instance_t instance, s1ap_nas_non_delivery_ind_t *s1ap_nas_non_delivery_ind) { return 0; }

int s1ap_eNB_handle_nas_first_req( instance_t instance, s1ap_nas_first_req_t *s1ap_nas_first_req_p) { return 0; }

int s1ap_eNB_initial_ctxt_resp( instance_t instance, s1ap_initial_context_setup_resp_t *initial_ctxt_resp_p) { return 0; }

int s1ap_eNB_ue_capabilities(instance_t instance, s1ap_ue_cap_info_ind_t *ue_cap_info_ind_p) { return 0; }

int s1ap_eNB_e_rab_setup_resp(instance_t instance, s1ap_e_rab_setup_resp_t *e_rab_setup_resp_p) { return 0; }

int s1ap_eNB_e_rab_modify_resp(instance_t instance, s1ap_e_rab_modify_resp_t *e_rab_modify_resp_p) { return 0; }

int s1ap_eNB_e_rab_release_resp(instance_t instance, s1ap_e_rab_release_resp_t *e_rab_release_resp_p) { return 0; }

int s1ap_eNB_path_switch_req(instance_t instance, s1ap_path_switch_req_t *path_switch_req_p) { return 0; }

int s1ap_eNB_generate_E_RAB_Modification_Indication( instance_t instance, s1ap_e_rab_modification_ind_t *e_rab_modification_ind) { return 0; }

