/* Auto-generated protocol stub for RISC-V port.
 * Replaces f1ap_cu_interface_management.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* f1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "f1ap_common.h"
#include "f1ap_cu_interface_management.h"
#include <stddef.h>

int CU_send_RESET(instance_t instance, F1AP_Reset_t *Reset) { return 0; }

int CU_handle_RESET_ACKKNOWLEDGE(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_RESET(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_RESET_ACKNOWLEDGE(instance_t instance, F1AP_ResetAcknowledge_t *ResetAcknowledge) { return 0; }

int CU_handle_ERROR_INDICATION(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_ERROR_INDICATION(instance_t instance, F1AP_ErrorIndication_t *ErrorIndication) { return 0; }

int CU_handle_F1_SETUP_REQUEST(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_F1_SETUP_RESPONSE(instance_t instance, f1ap_setup_resp_t *f1ap_setup_resp) { return 0; }

int CU_send_F1_SETUP_FAILURE(instance_t instance) { return 0; }

int CU_handle_gNB_DU_CONFIGURATION_UPDATE(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_gNB_DU_CONFIGURATION_FAILURE(instance_t instance, F1AP_GNBDUConfigurationUpdateFailure_t *GNBDUConfigurationUpdateFailure) { return 0; }

int CU_send_gNB_DU_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance, F1AP_GNBDUConfigurationUpdateAcknowledge_t *GNBDUConfigurationUpdateAcknowledge) { return 0; }

int CU_send_gNB_CU_CONFIGURATION_UPDATE(instance_t instance, f1ap_gnb_cu_configuration_update_t *f1ap_gnb_cu_configuration_update) { return 0; }

int CU_handle_gNB_CU_CONFIGURATION_UPDATE_FAILURE(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_gNB_CU_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_gNB_DU_RESOURCE_COORDINATION_REQUEST(instance_t instance, uint32_t assoc_id, uint32_t stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_gNB_DU_RESOURCE_COORDINATION_RESPONSE(instance_t instance, F1AP_GNBDUResourceCoordinationResponse_t *GNBDUResourceCoordinationResponse) { return 0; }

