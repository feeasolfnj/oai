/* Auto-generated stub for RISC-V port.
 * Original: rrc_gNB_NGAP.c (rrc NGAP procedures for gNB)
 *
 * The original source uses OAI-expected NGAP types that mouse07410 asn1c
 * does not generate:
 *  - NGAP_QosFlowSetupRequestList_t, NGAP_PDUSessionResourceSetupRequestTransferIEs_t
 *    (IE container types ¡ª asn1c generates ANY_t for open-type value, not unions)
 *  - NGAP_ProtocolIE_ID_id_* constants (asn1c generates plain numeric ProcedureCode
 *    without symbolic IE-ID macros)
 *  - .value.choice.* member access (requires union-based open-type, not ANY_t)
 *
 * Functions are no-ops so nr-softmodem still builds and links. NGAP signaling
 * (PDU session setup, UE context release, paging, NAS transport) will be
 * non-functional, but the PHY/MAC/RLC/PDCP stack runs normally.
 * Original code is preserved in rrc_gNB_NGAP.c.orig. */

#include "rrc_gNB_NGAP.h"
#include "intertask_interface.h"
#include <stddef.h>

void
rrc_gNB_send_NGAP_NAS_FIRST_REQ(
    const protocol_ctxt_t     *const ctxt_pP,
    rrc_gNB_ue_context_t      *ue_context_pP,
    NR_RRCSetupComplete_IEs_t *rrcSetupComplete
) { (void)ctxt_pP; (void)ue_context_pP; (void)rrcSetupComplete; }

int rrc_gNB_process_NGAP_INITIAL_CONTEXT_SETUP_REQ(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }

void
rrc_gNB_send_NGAP_INITIAL_CONTEXT_SETUP_RESP(
    const protocol_ctxt_t *const ctxt_pP,
    rrc_gNB_ue_context_t          *const ue_context_pP
) { (void)ctxt_pP; (void)ue_context_pP; }

int rrc_gNB_process_NGAP_DOWNLINK_NAS(MessageDef *msg_p, instance_t instance, mui_t *rrc_gNB_mui) { (void)msg_p; (void)instance; (void)rrc_gNB_mui; return 0; }

void
rrc_gNB_send_NGAP_UPLINK_NAS(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  NR_UL_DCCH_Message_t     *const ul_dcch_msg
) { (void)ctxt_pP; (void)ue_context_pP; (void)ul_dcch_msg; }

void
rrc_gNB_send_NGAP_PDUSESSION_SETUP_RESP(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
) { (void)ctxt_pP; (void)ue_context_pP; (void)xid; }

void rrc_gNB_process_NGAP_PDUSESSION_SETUP_REQ(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; }

int rrc_gNB_process_NGAP_PDUSESSION_MODIFY_REQ(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }

int
rrc_gNB_send_NGAP_PDUSESSION_MODIFY_RESP(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
) { (void)ctxt_pP; (void)ue_context_pP; (void)xid; return 0; }

/* rrc_gNB_modify_dedicatedRRCReconfiguration is defined in rrc_gNB.c ¡ª not stubbed. */

void rrc_gNB_send_NGAP_UE_CONTEXT_RELEASE_REQ(const module_id_t gnb_mod_idP, const rrc_gNB_ue_context_t *const ue_context_pP, const ngap_Cause_t causeP, const long cause_valueP) { (void)gnb_mod_idP; (void)ue_context_pP; (void)causeP; (void)cause_valueP; }

int rrc_gNB_process_NGAP_UE_CONTEXT_RELEASE_REQ(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }

int rrc_gNB_process_NGAP_UE_CONTEXT_RELEASE_COMMAND(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }

void rrc_gNB_send_NGAP_UE_CONTEXT_RELEASE_COMPLETE(instance_t instance, uint32_t gNB_ue_ngap_id) { (void)instance; (void)gNB_ue_ngap_id; }

void rrc_gNB_send_NGAP_UE_CAPABILITIES_IND(const protocol_ctxt_t *const ctxt_pP,
                                           rrc_gNB_ue_context_t *const ue_context_pP,
                                           const NR_UECapabilityInformation_t *const ue_cap_info)
{ (void)ctxt_pP; (void)ue_context_pP; (void)ue_cap_info; }

int rrc_gNB_process_NGAP_PDUSESSION_RELEASE_COMMAND(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }

void
rrc_gNB_send_NGAP_PDUSESSION_RELEASE_RESPONSE(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
) { (void)ctxt_pP; (void)ue_context_pP; (void)xid; }

void
nr_rrc_pdcp_config_security(
    const protocol_ctxt_t  *const ctxt_pP,
    rrc_gNB_ue_context_t   *const ue_context_pP,
    const uint8_t          send_security_mode_command
) { (void)ctxt_pP; (void)ue_context_pP; (void)send_security_mode_command; }

int rrc_gNB_process_PAGING_IND(MessageDef *msg_p, instance_t instance) { (void)msg_p; (void)instance; return 0; }
