#  Example command line
#  "python.exe" pdaf_tolerances_tuning_AR1630.py --cfg-file rec_tol_tuning.ini -rel DNP.raw -rr DNP_red.raw -rb DNP_blue.raw -o pdaf_tolerances.ini


import os
import sys


def keyword(line):
    sym = line.split('=', 1)
    if (len(sym) < 2):
        return None
    return sym[0].strip()

def config_merge(merge_from, merge_to):
    for fr_line in merge_from:
        fr_keyword = keyword(fr_line)
        if (fr_keyword == None):
            continue
        for i in range(len(merge_to)):
            to_keyword = keyword(merge_to[i])
            if (to_keyword == None):
                continue
            if (to_keyword != fr_keyword):
                continue
            merge_to[i] = fr_line

def output_of_cmd(cmd):
    print(cmd)
    cmd_output = []
    with os.popen(cmd, mode='r', buffering=-1) as p:
        for line in p:
            cmd_output.append(line)
    return cmd_output

def read_lines_from_file(filename):
    lines = []
    with open(filename, 'r') as f:
        for line in f:
            lines.append(line)
    return lines

def write_lines_to_file(lines, filename):
    with open(filename, 'w') as f:
        for line in lines:
            print(line, file=f, end='')

addr_of = {
    'BPC_PDAF_REC_TOL_RR10'  : 0x3CAE,
    'BPC_PDAF_REC_TOL_RR32'  : 0x3CB0,
    'BPC_PDAF_REC_TOL_RR54'  : 0x3CB2,
    'BPC_PDAF_REC_TOL_RB10'  : 0x3CB4,
    'BPC_PDAF_REC_TOL_RB32'  : 0x3CB6,
    'BPC_PDAF_REC_TOL_RB54'  : 0x3CB8,
    'BPC_PDAF_REC_TOL_REL10' : 0x3CBA,
    'BPC_PDAF_REC_TOL_REL32' : 0x3CBC,
    'BPC_PDAF_REC_TOL_REL54' : 0x3CBE }
def parse_reg(line):
    ra = ''.join(line.split('=')[1].split()).split(',')
    if ra[0] not in addr_of:
        return None
    ra[0] = hex(addr_of[ra[0]])
    return ra

def rec_tol_tuning_procedure(base_rtt_cfg_filename, rel_raw_filename, rr_raw_filename, rb_raw_filename, output_rtt_filename):

    #  Path for temporary rec_tol_tuning.ini with our edits
    rtt_split = os.path.split(base_rtt_cfg_filename)
    rtt_cfg_filename = os.path.join(rtt_split[0], 'temp_' + rtt_split[1])

    #  The various command lines that we will need for the rest of this procedure
    psc_cmd = 'pdaf_sc_calib.exe {0} . -l -oc 42 384 216 96'.format(rel_tuning_raw_filename)
    rtt_rw_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rel -c 0.2 -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rel_raw_filename)
    rtt_hw_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rel -c 0.2 -r 100 -b 42 42 -i {1}'.format(rtt_cfg_filename, rel_raw_filename)
    rtt_rr_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rr  -c 0.2 -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rr_raw_filename)
    rtt_rb_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rb  -c 0.2 -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rb_raw_filename)
    rtt_fb_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rb         -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rb_raw_filename)
    rtt_fr_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rr         -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rr_raw_filename)
    rtt_fw_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rel        -r 300 -b 42 42 -i {1}'.format(rtt_cfg_filename, rel_raw_filename)
    rtt_sw_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rel      -r 99999 -b 42 42 -i {1} -o sim_{1} .'.format(rtt_cfg_filename, rel_raw_filename)
    rtt_sr_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rr       -r 99999 -b 42 42 -i {1} -o sim_{1} .'.format(rtt_cfg_filename, rr_raw_filename)
    rtt_sb_cmd = 'rec_tol_tuning.exe --cfg-file {0} -m rb       -r 99999 -b 42 42 -i {1} -o sim_{1} .'.format(rtt_cfg_filename, rb_raw_filename)

    #  Run pdaf_sc_calib.exe and merge the output parameters into rec_tol_tuning.ini
    print('Runing spatial correction')
    pdaf_sc_calib_output = output_of_cmd(psc_cmd)
    rec_tol_tuning = read_lines_from_file(base_rtt_cfg_filename)
    config_merge(pdaf_sc_calib_output, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    print('Tuning white - first pass')
    rtt_rw_cmd_output = output_of_cmd(rtt_rw_cmd)

    print('Tuning white - high target') # used for first pass red & blue
    rtt_hw_cmd_output = output_of_cmd(rtt_hw_cmd)

    #  From the hw ("high white") output just get the 'pdaf_rec_tol_rel' parameters and
    #  merge into rec_tol_tuning.ini
    pdaf_rec_tol_rel = []
    for line in rtt_hw_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rel':
            pdaf_rec_tol_rel.append(line)
    config_merge(pdaf_rec_tol_rel, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    #  Run the rr and rb tuning
    print('Tuning red - first pass')
    rtt_rr_cmd_output = output_of_cmd(rtt_rr_cmd)
    pdaf_rec_tol_rr = []
    for line in rtt_rr_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rr':
            pdaf_rec_tol_rr.append(line)
    config_merge(pdaf_rec_tol_rr, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    print('Tuning blue - first pass')
    rtt_rb_cmd_output = output_of_cmd(rtt_rb_cmd)
    pdaf_rec_tol_rb = []
    for line in rtt_rb_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rr':
            pdaf_rec_tol_rb.append(line)
    config_merge(pdaf_rec_tol_rb, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    #  From the rw ("conservative white") output just get the 'pdaf_rec_tol_rel' parameters and
    #  merge into rec_tol_tuning.ini
    pdaf_rec_tol_rel = []
    for line in rtt_rw_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rel':
            pdaf_rec_tol_rel.append(line)
    config_merge(pdaf_rec_tol_rel, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    print('Tuning blue - final pass')
    rtt_fb_cmd_output = output_of_cmd(rtt_fb_cmd)
    pdaf_rec_tol_fb = []
    for line in rtt_fb_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rb':
            pdaf_rec_tol_fb.append(line)
    config_merge(pdaf_rec_tol_fb, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    print('Tuning red - final pass')
    rtt_fr_cmd_output = output_of_cmd(rtt_fr_cmd)
    pdaf_rec_tol_fr = []
    for line in rtt_fr_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rr':
            pdaf_rec_tol_fr.append(line)
    config_merge(pdaf_rec_tol_fr, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)

    print('Tuning white - final pass')
    rtt_fw_cmd_output = output_of_cmd(rtt_fw_cmd)
    pdaf_rec_tol_fr = []
    for line in rtt_fw_cmd_output:
        if keyword(line) == 'pdaf_rec_tol_rel':
            pdaf_rec_tol_fr.append(line)
    config_merge(pdaf_rec_tol_fr, rec_tol_tuning)
    write_lines_to_file(rec_tol_tuning, rtt_cfg_filename)


    #  Extract the FIELD_WR= commands from the outputs of the 0.80 rel
    #  tuning and the rr and rb tuning
    tuning_regs = [] # list of 2-item lists of [addr,data]
    for r in rtt_fw_cmd_output:
        if r.startswith('FIELD_WR'):
            ra = parse_reg(r)
            if ra != None:
                tuning_regs.append(parse_reg(r))
    for r in rtt_fr_cmd_output:
        if r.startswith('FIELD_WR'):
            ra = parse_reg(r)
            if ra != None:
                tuning_regs.append(parse_reg(r))
    for r in rtt_fb_cmd_output:
        if r.startswith('FIELD_WR'):
            ra = parse_reg(r)
            if ra != None:
                tuning_regs.append(parse_reg(r))
    tuning_regs.sort(key= lambda x : x[0]) # sort by addr

    #  Output register values
    with open(output_rtt_filename, 'w') as f:
        print('[PDAF Tolerances]', file=f)
        for r in tuning_regs:
            print('REG= {0}, {1}'.format(r[0], r[1]), file=f)

        print('', file=f)
        print('[PDAF Tolerances - OTPM Load]', file=f)
        print('REG= 0x3800, 0xF803 // Set address', file=f)
        print('REG= 0x3802, 0x4E57 // Change address', file=f)
        addr = 0x3804
        for r in tuning_regs:
            print('REG= 0x{0:04X}, {1} //'.format(addr, r[1]), file=f)
            addr += 2
        print('REG= 0x{0:04X}, 0xFFFF // Termination word'.format(addr), file=f)

    if simulate:
      print('Simulating results')
      output_of_cmd(rtt_sw_cmd)
      output_of_cmd(rtt_sr_cmd)
      output_of_cmd(rtt_sb_cmd)


base_rec_tol_tuning_filename = 'rec_tol_tuning.ini'
rel_tuning_raw_filename = 'DNP.raw'
rr_tuning_raw_filename = 'DNP_red.raw'
rb_tuning_raw_filename = 'DNP_blue.raw'
output_tol_tuning_filename = 'pdaf_tolerances.ini'
simulate = 0


#  Command parameters
#    --cfg-file rec_tol_tuning.ini
#    -rel DNP.raw
#    -rr DNP_red.raw
#    -rb DNP_blue.raw
#    -o pdaf_tolerances.ini

for i in range(len(sys.argv) - 1):
    sw = sys.argv[i]
    arg = sys.argv[i + 1]
    if sw == '--cfg-file' or sw == '-cfg':
        base_rec_tol_tuning_filename = arg
        i += 1
    if sw == '-rel':
        rel_tuning_raw_filename = arg
        i += 1
    if sw == '-rr':
        rr_tuning_raw_filename = arg
        i += 1
    if sw == '-rb':
        rb_tuning_raw_filename = arg
        i += 1
    if sw == '-o':
        output_tol_tuning_filename = arg
        i += 1
    if sw == '-s':
        simulate = 1
        i += 1
        

#  Run tuning procedure
rec_tol_tuning_procedure(base_rec_tol_tuning_filename, rel_tuning_raw_filename, rr_tuning_raw_filename, rb_tuning_raw_filename, output_tol_tuning_filename)
