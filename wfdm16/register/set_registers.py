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
        #board_version = get_board_version(ip_address)
        board_version = 2

        # reset ADC (soft reset)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x100, '>B', 0x3c)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x1)
        rbcp.write_register_f(ip_address, 0x100, '>B', 0x3c)

        # internal power down mode (do reset)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x108, '>B', 0x3)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x1)
        rbcp.write_register_f(ip_address, 0x108, '>B', 0x3)
        time.sleep(0.1)

        # internal power down mode (do release reset)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x108, '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x10,  '>B', 0x1)
        rbcp.write_register_f(ip_address, 0x108, '>B', 0x0)

        if board_version == 1:
            # serial outout number of bits (set 12 bits)
            rbcp.write_register_f(ip_address, 0x10,  '>B', 0x0)
            rbcp.write_register_f(ip_address, 0x121, '>B', 0x3)
            rbcp.write_register_f(ip_address, 0x1ff, '>B', 0x1)
            rbcp.write_register_f(ip_address, 0x10,  '>B', 0x1)
            rbcp.write_register_f(ip_address, 0x121, '>B', 0x3)
            rbcp.write_register_f(ip_address, 0x1ff, '>B', 0x1)
            
        # Output format: offset binary
        rbcp.write_register_f(ip_address, 0x10, '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x114, '>B', 0x0)
        rbcp.write_register_f(ip_address, 0x10, '>B', 0x1)
        rbcp.write_register_f(ip_address, 0x114, '>B', 0x0)

        # fpga adc interface reset
        rbcp.write_register_f(ip_address, 0x5, '>B', 0x1)
        rbcp.write_register_f(ip_address, 0x5, '>B', 0x0)
        
	#Window Size
	rbcp.write_register_f(ip_address, 0x6, '>B', 0x7f)
	#Delay
	rbcp.write_register_f(ip_address, 0x7, '>B', 0x7f)

        #SelfTriggerEnable
	rbcp.write_register_f(ip_address, 0x18, '>B', 0xff)
	rbcp.write_register_f(ip_address, 0x19, '>B', 0xff)

        #SelfTriggerPolarity
	rbcp.write_register_f(ip_address, 0x1A, '>B', 0x00)
	rbcp.write_register_f(ip_address, 0x1B, '>B', 0x00)

        #SelfTriggerDelay
	rbcp.write_register_f(ip_address, 0x1C, '>B', 0x00)

        #SelfTrigger
	rbcp.write_register_f(ip_address, 0x20, '>B', 0x00) # ch 0
	rbcp.write_register_f(ip_address, 0x21, '>B', 0x00) # ch 0
	rbcp.write_register_f(ip_address, 0x22, '>B', 0x00) # ch 1
	rbcp.write_register_f(ip_address, 0x23, '>B', 0x00) # ch 1
	rbcp.write_register_f(ip_address, 0x24, '>B', 0x00) # ch 2
	rbcp.write_register_f(ip_address, 0x25, '>B', 0x01) # ch 2
	rbcp.write_register_f(ip_address, 0x26, '>B', 0x00) # ch 3
	rbcp.write_register_f(ip_address, 0x27, '>B', 0x00) # ch 3
	rbcp.write_register_f(ip_address, 0x28, '>B', 0x00) # ch 4
	rbcp.write_register_f(ip_address, 0x29, '>B', 0x00) # ch 4
	rbcp.write_register_f(ip_address, 0x2A, '>B', 0x00) # ch 5
	rbcp.write_register_f(ip_address, 0x2B, '>B', 0x00) # ch 5
	rbcp.write_register_f(ip_address, 0x2C, '>B', 0x00) # ch 6
	rbcp.write_register_f(ip_address, 0x2D, '>B', 0x00) # ch 6
	rbcp.write_register_f(ip_address, 0x2E, '>B', 0x00) # ch 7
	rbcp.write_register_f(ip_address, 0x2F, '>B', 0x00) # ch 7
	rbcp.write_register_f(ip_address, 0x30, '>B', 0x00) # ch 8
	rbcp.write_register_f(ip_address, 0x31, '>B', 0x00) # ch 8
	rbcp.write_register_f(ip_address, 0x32, '>B', 0x00) # ch 9
	rbcp.write_register_f(ip_address, 0x33, '>B', 0x00) # ch 9
	rbcp.write_register_f(ip_address, 0x34, '>B', 0x00) # ch 10
	rbcp.write_register_f(ip_address, 0x35, '>B', 0x00) # ch 10
	rbcp.write_register_f(ip_address, 0x36, '>B', 0x00) # ch 11
	rbcp.write_register_f(ip_address, 0x37, '>B', 0x00) # ch 11
	rbcp.write_register_f(ip_address, 0x38, '>B', 0x00) # ch 12
	rbcp.write_register_f(ip_address, 0x39, '>B', 0x00) # ch 12
	rbcp.write_register_f(ip_address, 0x3A, '>B', 0x00) # ch 13
	rbcp.write_register_f(ip_address, 0x3B, '>B', 0x00) # ch 13
	rbcp.write_register_f(ip_address, 0x3C, '>B', 0x00) # ch 14
	rbcp.write_register_f(ip_address, 0x3D, '>B', 0x00) # ch 14
	rbcp.write_register_f(ip_address, 0x3E, '>B', 0x00) # ch 15
	rbcp.write_register_f(ip_address, 0x3F, '>B', 0x00) # ch 15
        
if __name__ == '__main__':
    main()
