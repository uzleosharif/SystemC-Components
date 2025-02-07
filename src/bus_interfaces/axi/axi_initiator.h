#pragma once

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <axi/axi_tlm.h>
#include <axi/pe/simple_initiator.h>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

namespace axi {
/**
 * @brief axi_initiator class provides an input_socket for incoming TLM transactions.
 * It attaches AXI extension to the tlm_generic_payload and forwards it to the AXI Protocol Engine.
 */
class axi_initiator_base : public sc_core::sc_module {
public:
    sc_core::sc_in<bool> clk_i{"clk_i"};
    tlm_utils::simple_target_socket<axi_initiator_base> input_socket{"input_socket"};

    /**
     * Create and attach AXI extension.
     * AXI protocol engine expects the incoming transactions to be controlled by a memory manager.
     * If transaction does not have an associated memory manager, copy the payload into a new one with mm.
     */
    tlm::tlm_generic_payload* create_axi_trans(tlm::tlm_generic_payload& p);

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);

    axi_initiator_base(const sc_core::sc_module_name& nm, axi::pe::simple_initiator_b& pe, uint32_t width);
    virtual ~axi_initiator_base(){};

private:
    axi::pe::simple_initiator_b& pe;
    uint32_t buswidth{0};
    unsigned id{0};
};

template <unsigned int BUSWIDTH = 32> class axi_initiator : public axi_initiator_base {
public:
    axi::axi_initiator_socket<BUSWIDTH> intor{"intor"};

    axi_initiator(sc_core::sc_module_name nm)
    : axi_initiator_base(nm, intor_pe, BUSWIDTH) {
        intor_pe.clk_i(clk_i);
    };

    virtual ~axi_initiator(){};

private:
    axi::pe::simple_axi_initiator<BUSWIDTH> intor_pe{"intor_pe", intor};
};

} // namespace axi
