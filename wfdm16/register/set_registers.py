#!/usr/bin/env python

import time
import SitcpRbcp

def get_board_version(ip_address):
    print ip_address
    rbcp = SitcpRbcp.SitcpRbcp()
    reg_value = rbcp.read_register_f(ip_address, 0x4, '>B')
    if reg_value == 0x02:
        print 'V2 board'
        return 2 
    else:
        print 'V1 board'
        return 1

def main():
    rbcp = SitcpRbcp.SitcpRbcp()
    #rbcp.set_verify_mode()
    rbcp.set_timeout(1.0)
    ip_address_list = [
        '192.168.10.16',
#        '192.168.10.17'
    ]
    
    for ip_address in ip_address_list:
        board_version = 2

        print 'Set registers...'
        
        #######################################################
        # control register (0x00000004)
        #######################################################
        # bit7: OFSET_BIN ...
        # bit6: EDGE_SEL  ... 0 : positive signal, 1 : negative signal
        # bit5: TRIG_SEL  ... 00: NIM_IN 1, 01: NIM_IN 2
        # bit4: TRIG_SEL  ... 10: LOGICAL OR, 11: LOGICAL AND
        # bit3: UPCH_SEL  ... if COMBINE=0,enable 0-15ch,else-> 0 : enable 0-7ch, 1 : enable 8-15ch,
        # bit2: COMBINE   ... 0 : 2048 sample, 1 : 4096 sample 
        # bit1: RATE      ... 00: 40MHz, 01: 20MHz
        # bit0: RATE      ... 10: 10MHz, 11:  5MHz

        rbcp.write_register_f(ip_address, 0x4,  '>B', 0x20) # 40MHz, self, 2048clock, ch0-7 -> Fe
        #rbcp.write_register_f(ip_address, 0x4,  '>B', 0x00)# 40MHz, ext1, 2048clock, ch0-7 -> muon

        #######################################################
        # trigger position register (0x00000008, 0x00000009)
        #######################################################
        rbcp.write_register_f(ip_address, 0x8,  '>B', 0x06)
        rbcp.write_register_f(ip_address, 0x9,  '>B', 0x0C)
        
        #######################################################
        # trigger enable register (0x0000000A, 0x0000000B)
        # 0x0000000A : ch [15:8]
        # 0x0000000B : ch [ 7:0]
        #######################################################
        rbcp.write_register_f(ip_address, 0xA,  '>B', 0x00)
        #rbcp.write_register_f(ip_address, 0xB,  '>B', 0xff)
        rbcp.write_register_f(ip_address, 0xB,  '>B', 0x0f) # Anode4ch

        #######################################################
        # trigger invert register (0x0000000C, 0x0000000D)
        #######################################################
        rbcp.write_register_f(ip_address, 0xC,  '>B', 0x00)
        rbcp.write_register_f(ip_address, 0xD,  '>B', 0x00)

        #######################################################
        # Vth register (0x00000020, 0x0000003F)
        #######################################################

        ## ch 0
        rbcp.write_register_f(ip_address, 0x20,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x21,  '>B', 0x00)
        #rbcp.write_register_f(ip_address, 0x20,  '>B', 0x03)
        #rbcp.write_register_f(ip_address, 0x21,  '>B', 0x40)

        ## ch 1
        rbcp.write_register_f(ip_address, 0x22,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x23,  '>B', 0xC0)
        #rbcp.write_register_f(ip_address, 0x22,  '>B', 0x03)
        #rbcp.write_register_f(ip_address, 0x23,  '>B', 0x30)

        ## ch 2
        #rbcp.write_register_f(ip_address, 0x24,  '>B', 0x03)
        #rbcp.write_register_f(ip_address, 0x25,  '>B', 0x80)
        rbcp.write_register_f(ip_address, 0x24,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x25,  '>B', 0xE0)

        ## ch 3
        rbcp.write_register_f(ip_address, 0x26,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x27,  '>B', 0x50)
        #rbcp.write_register_f(ip_address, 0x26,  '>B', 0x02)
        #rbcp.write_register_f(ip_address, 0x27,  '>B', 0xB0)

        ## ch 4
        rbcp.write_register_f(ip_address, 0x28,  '>B', 0x04)
        rbcp.write_register_f(ip_address, 0x29,  '>B', 0x80)

        ## ch 5
        rbcp.write_register_f(ip_address, 0x2A,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x2B,  '>B', 0xf0)

        ## ch 6
        rbcp.write_register_f(ip_address, 0x2C,  '>B', 0x01)
        rbcp.write_register_f(ip_address, 0x2D,  '>B', 0x40)

        ## ch 7
        rbcp.write_register_f(ip_address, 0x2E,  '>B', 0x02)
        rbcp.write_register_f(ip_address, 0x2F,  '>B', 0x20)

        
        rbcp.write_register_f(ip_address, 0x30,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x31,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x32,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x33,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x34,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x35,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x36,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x37,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x38,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x39,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x3A,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x3B,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x3C,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x3D,  '>B', 0xf0)

        rbcp.write_register_f(ip_address, 0x3E,  '>B', 0x0f)
        rbcp.write_register_f(ip_address, 0x3F,  '>B', 0xf0)
        
        print 'Done!'        

        
if __name__ == '__main__':
    main()
