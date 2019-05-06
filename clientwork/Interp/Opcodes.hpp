//	opcodes.hpp
#ifndef OPCODES_HPP
#define OPCODES_HPP

const uchar op_bnot			=	0x00;
const uchar op_add			=	0x02;
const uchar op_sub			=	0x04;
const uchar op_mul			=	0x06;
const uchar op_div			=	0x08;
const uchar op_mod			=	0x0A;
const uchar op_shr			=	0x0C;
const uchar op_shl			=	0x0E;
const uchar op_xor			=	0x10;
const uchar op_and			=	0x12;
const uchar op_or			=	0x14;
const uchar op_neg			=	0x16;
const uchar op_not			=	0x18;
const uchar op_eq			=	0x1A;
const uchar op_ne			=	0x1C;
const uchar op_gt			=	0x1E;
const uchar op_ge			=	0x20;
const uchar op_lt			=	0x22;
const uchar op_le			=	0x24;
const uchar op_ugt			=	0x26;
const uchar op_uge			=	0x28;
const uchar op_ult			=	0x2A;
const uchar op_ule			=	0x2C;
const uchar op_bt			=	0x2E;
const uchar op_bt_word		=	0x2E;
const uchar op_bt_byte		=	0x2F;
const uchar op_bnt			=	0x30;
const uchar op_bnt_word		=	0x30;
const uchar op_bnt_byte		=	0x31;
const uchar op_jmp_word		=	0x32;
const uchar op_jmp_byte		=	0x33;
const uchar op_loadi_word	=	0x34;
const uchar op_loadi_byte	=	0x35;
const uchar op_push			=	0x36;
const uchar op_pushi_word	=	0x38;
const uchar op_pushi_byte	=	0x39;
const uchar op_toss			=	0x3A;
const uchar op_dup			=	0x3C;
const uchar op_link_word	=	0x3E;
const uchar op_link_byte	=	0x3F;
const uchar op_call			=	0x40;
const uchar op_call_word	=	0x40;
const uchar op_call_byte	=	0x41;
const uchar op_callk		=	0x42;
const uchar op_callk_word	=	0x42;
const uchar op_callk_byte	=	0x43;
const uchar op_callb		=	0x44;
const uchar op_callb_word	=	0x44;
const uchar op_callb_byte	=	0x45;
const uchar op_calle		=	0x46;
const uchar op_calle_word	=	0x46;
const uchar op_calle_byte	=	0x47;
const uchar op_ret			=	0x48;
const uchar op_send			=	0x4A;
const uchar op_send_byte	=	0x4A;
const uchar op_class_word	=	0x50;
const uchar op_class_byte	=	0x51;
const uchar op_self			=	0x54;
const uchar op_super		=	0x56;
const uchar op_super_word	=	0x56;
const uchar op_super_byte	=	0x57;
const uchar op_rest_byte	=	0x59;
const uchar op_lea_word		=	0x5A;
const uchar op_lea_byte		=	0x5B;
const uchar op_selfID		=	0x5C;
const uchar op_pprev		=	0x60;
const uchar op_pToa_word	=	0x62;
const uchar op_pToa_byte	=	0x63;
const uchar op_aTop_word	=	0x64;
const uchar op_aTop_byte	=	0x65;
const uchar op_pTos_word	=	0x66;
const uchar op_pTos_byte	=	0x67;
const uchar op_sTop_word	=	0x68;
const uchar op_sTop_byte	=	0x69;
const uchar op_ipToa_word	=	0x6A;
const uchar op_ipToa_byte	=	0x6B;
const uchar op_dpToa_word	=	0x6C;
const uchar op_dpToa_byte	=	0x6D;
const uchar op_ipTos_word	=	0x6E;
const uchar op_ipTos_byte	=	0x6F;
const uchar op_dpTos_word	=	0x70;
const uchar op_dpTos_byte	=	0x71;
const uchar op_lofsa		=	0x72;
const uchar op_lofss		=	0x74;
const uchar op_push0		=	0x76;
const uchar op_push1		=	0x78;
const uchar op_push2		=	0x7A;
const uchar op_pushSelf		=	0x7C;
const uchar op_fileName		=	0x7D;
const uchar op_lineNum		=	0x7E;
const uchar op_lag_word		=	0x80;
const uchar op_lag_byte		=	0x81;
const uchar op_lal_word		=	0x82;
const uchar op_lal_byte		=	0x83;
const uchar op_lat_word		=	0x84;
const uchar op_lat_byte		=	0x85;
const uchar op_lap_word		=	0x86;
const uchar op_lap_byte		=	0x87;
const uchar op_lsg_word		=	0x88;
const uchar op_lsg_byte		=	0x89;
const uchar op_lsl_word		=	0x8A;
const uchar op_lsl_byte		=	0x8B;
const uchar op_lst_word		=	0x8C;
const uchar op_lst_byte		=	0x8D;
const uchar op_lsp_word		=	0x8E;
const uchar op_lsp_byte		=	0x8F;
const uchar op_lagi_word	=	0x90;
const uchar op_lagi_byte	=	0x91;
const uchar op_lali_word	=	0x92;
const uchar op_lali_byte	=	0x93;
const uchar op_lati_word	=	0x94;
const uchar op_lati_byte	=	0x95;
const uchar op_lapi_word	=	0x96;
const uchar op_lapi_byte	=	0x97;
const uchar op_lsgi_word	=	0x98;
const uchar op_lsgi_byte	=	0x99;
const uchar op_lsli_word	=	0x9A;
const uchar op_lsli_byte	=	0x9B;
const uchar op_lsti_word	=	0x9C;
const uchar op_lsti_byte	=	0x9D;
const uchar op_lspi_word	=	0x9E;
const uchar op_lspi_byte	=	0x9F;
const uchar op_sag_word		=	0xA0;
const uchar op_sag_byte		=	0xA1;
const uchar op_sal_word		=	0xA2;
const uchar op_sal_byte		=	0xA3;
const uchar op_sat_word		=	0xA4;
const uchar op_sat_byte		=	0xA5;
const uchar op_sap_word		=	0xA6;
const uchar op_sap_byte		=	0xA7;
const uchar op_ssg_word		=	0xA8;
const uchar op_ssg_byte		=	0xA9;
const uchar op_ssl_word		=	0xAA;
const uchar op_ssl_byte		=	0xAB;
const uchar op_sst_word		=	0xAC;
const uchar op_sst_byte		=	0xAD;
const uchar op_ssp_word		=	0xAE;
const uchar op_ssp_byte		=	0xAF;
const uchar op_sagi_word	=	0xB0;
const uchar op_sagi_byte	=	0xB1;
const uchar op_sali_word	=	0xB2;
const uchar op_sali_byte	=	0xB3;
const uchar op_sati_word	=	0xB4;
const uchar op_sati_byte	=	0xB5;
const uchar op_sapi_word	=	0xB6;
const uchar op_sapi_byte	=	0xB7;
const uchar op_ssgi_word	=	0xB8;
const uchar op_ssgi_byte	=	0xB9;
const uchar op_ssli_word	=	0xBA;
const uchar op_ssli_byte	=	0xBB;
const uchar op_ssti_word	=	0xBC;
const uchar op_ssti_byte	=	0xBD;
const uchar op_sspi_word	=	0xBE;
const uchar op_sspi_byte	=	0xBF;
const uchar op_iag_word		=	0xC0;
const uchar op_iag_byte		=	0xC1;
const uchar op_ial_word		=	0xC2;
const uchar op_ial_byte		=	0xC3;
const uchar op_iat_word		=	0xC4;
const uchar op_iat_byte		=	0xC5;
const uchar op_iap_word		=	0xC6;
const uchar op_iap_byte		=	0xC7;
const uchar op_isg_word		=	0xC8;
const uchar op_isg_byte		=	0xC9;
const uchar op_isl_word		=	0xCA;
const uchar op_isl_byte		=	0xCB;
const uchar op_ist_word		=	0xCC;
const uchar op_ist_byte		=	0xCD;
const uchar op_isp_word		=	0xCE;
const uchar op_isp_byte		=	0xCF;
const uchar op_iagi_word	=	0xD0;
const uchar op_iagi_byte	=	0xD1;
const uchar op_iali_word	=	0xD2;
const uchar op_iali_byte	=	0xD3;
const uchar op_iati_word	=	0xD4;
const uchar op_iati_byte	=	0xD5;
const uchar op_iapi_word	=	0xD6;
const uchar op_iapi_byte	=	0xD7;
const uchar op_isgi_word	=	0xD8;
const uchar op_isgi_byte	=	0xD9;
const uchar op_isli_word	=	0xDA;
const uchar op_isli_byte	=	0xDB;
const uchar op_isti_word	=	0xDC;
const uchar op_isti_byte	=	0xDD;
const uchar op_ispi_word	=	0xDE;
const uchar op_ispi_byte	=	0xDF;
const uchar op_dag_word		=	0xE0;
const uchar op_dag_byte		=	0xE1;
const uchar op_dal_word		=	0xE2;
const uchar op_dal_byte		=	0xE3;
const uchar op_dat_word		=	0xE4;
const uchar op_dat_byte		=	0xE5;
const uchar op_dap_word		=	0xE6;
const uchar op_dap_byte		=	0xE7;
const uchar op_dsg_word		=	0xE8;
const uchar op_dsg_byte		=	0xE9;
const uchar op_dsl_word		=	0xEA;
const uchar op_dsl_byte		=	0xEB;
const uchar op_dst_word		=	0xEC;
const uchar op_dst_byte		=	0xED;
const uchar op_dsp_word		=	0xEE;
const uchar op_dsp_byte		=	0xEF;
const uchar op_dagi_word	=	0xF0;
const uchar op_dagi_byte	=	0xF1;
const uchar op_dali_word	=	0xF2;
const uchar op_dali_byte	=	0xF3;
const uchar op_dati_word	=	0xF4;
const uchar op_dati_byte	=	0xF5;
const uchar op_dapi_word	=	0xF6;
const uchar op_dapi_byte	=	0xF7;
const uchar op_dsgi_word	=	0xF8;
const uchar op_dsgi_byte	=	0xF9;
const uchar op_dsli_word	=	0xFA;
const uchar op_dsli_byte	=	0xFB;
const uchar op_dsti_word	=	0xFC;
const uchar op_dsti_byte	=	0xFD;
const uchar op_dspi_word	=	0xFE;
const uchar op_dspi_byte	=	0xFF;

#define OPbnot			00h
#define OPadd			02h
#define OPsub			04h
#define OPmul			06h
#define OPdiv			08h
#define OPmod			0Ah
#define OPshr			0Ch
#define OPshl			0Eh
#define OPxor			10h
#define OPand			12h
#define OPor			14h
#define OPneg			16h
#define OPnot			18h
#define OPeq			1Ah
#define OPne			1Ch
#define OPgt			1Eh
#define OPge			20h
#define OPlt			22h
#define OPle			24h
#define OPugt			26h
#define OPuge			28h
#define OPult			2Ah
#define OPule			2Ch
#define OPbt			2Eh
#define OPbt_word		2Eh
#define OPbt_byte		2Fh
#define OPbnt			30h
#define OPbnt_word		30h
#define OPbnt_byte		31h
#define OPjmp_word		32h
#define OPjmp_byte		33h
#define OPloadi_word	34h
#define OPloadi_byte	35h
#define OPpush			36h
#define OPpushi_word	38h
#define OPpushi_byte	39h
#define OPtoss			3Ah
#define OPdup			3Ch
#define OPlink_word	3Eh
#define OPlink_byte	3Fh
#define OPcall			40h
#define OPcall_word	40h
#define OPcall_byte	41h
#define OPcallk		42h
#define OPcallk_word	42h
#define OPcallk_byte	43h
#define OPcallb		44h
#define OPcallb_word	44h
#define OPcallb_byte	45h
#define OPcalle		46h
#define OPcalle_word	46h
#define OPcalle_byte	47h
#define OPret			48h
#define OPsend			4Ah
#define OPsend_byte	4Ah
#define OPclass_word	50h
#define OPclass_byte	51h
#define OPself			54h
#define OPsuper		56h
#define OPsuper_word	56h
#define OPsuper_byte	57h
#define OPrest_byte	59h
#define OPlea_word		5Ah
#define OPlea_byte		5Bh
#define OPselfID		5Ch
#define OPpprev		60h
#define OPpToa_word	62h
#define OPpToa_byte	63h
#define OPaTop_word	64h
#define OPaTop_byte	65h
#define OPpTos_word	66h
#define OPpTos_byte	67h
#define OPsTop_word	68h
#define OPsTop_byte	69h
#define OPipToa_word	6Ah
#define OPipToa_byte	6Bh
#define OPdpToa_word	6Ch
#define OPdpToa_byte	6Dh
#define OPipTos_word	6Eh
#define OPipTos_byte	6Fh
#define OPdpTos_word	70h
#define OPdpTos_byte	71h
#define OPlofsa		72h
#define OPlofss		74h
#define OPpush0		76h
#define OPpush1		78h
#define OPpush2		7Ah
#define OPpushSelf		7Ch
#define OPfileName		7Dh
#define OPlineNum		7Eh
#define OPlag_word		80h
#define OPlag_byte		81h
#define OPlal_word		82h
#define OPlal_byte		83h
#define OPlat_word		84h
#define OPlat_byte		85h
#define OPlap_word		86h
#define OPlap_byte		87h
#define OPlsg_word		88h
#define OPlsg_byte		89h
#define OPlsl_word		8Ah
#define OPlsl_byte		8Bh
#define OPlst_word		8Ch
#define OPlst_byte		8Dh
#define OPlsp_word		8Eh
#define OPlsp_byte		8Fh
#define OPlagi_word	90h
#define OPlagi_byte	91h
#define OPlali_word	92h
#define OPlali_byte	93h
#define OPlati_word	94h
#define OPlati_byte	95h
#define OPlapi_word	96h
#define OPlapi_byte	97h
#define OPlsgi_word	98h
#define OPlsgi_byte	99h
#define OPlsli_word	9Ah
#define OPlsli_byte	9Bh
#define OPlsti_word	9Ch
#define OPlsti_byte	9Dh
#define OPlspi_word	9Eh
#define OPlspi_byte	9Fh
#define OPsag_word		A0h
#define OPsag_byte		A1h
#define OPsal_word		A2h
#define OPsal_byte		A3h
#define OPsat_word		A4h
#define OPsat_byte		A5h
#define OPsap_word		A6h
#define OPsap_byte		A7h
#define OPssg_word		A8h
#define OPssg_byte		A9h
#define OPssl_word		AAh
#define OPssl_byte		ABh
#define OPsst_word		ACh
#define OPsst_byte		ADh
#define OPssp_word		AEh
#define OPssp_byte		AFh
#define OPsagi_word	B0h
#define OPsagi_byte	B1h
#define OPsali_word	B2h
#define OPsali_byte	B3h
#define OPsati_word	B4h
#define OPsati_byte	B5h
#define OPsapi_word	B6h
#define OPsapi_byte	B7h
#define OPssgi_word	B8h
#define OPssgi_byte	B9h
#define OPssli_word	BAh
#define OPssli_byte	BBh
#define OPssti_word	BCh
#define OPssti_byte	BDh
#define OPsspi_word	BEh
#define OPsspi_byte	BFh
#define OPiag_word		C0h
#define OPiag_byte		C1h
#define OPial_word		C2h
#define OPial_byte		C3h
#define OPiat_word		C4h
#define OPiat_byte		C5h
#define OPiap_word		C6h
#define OPiap_byte		C7h
#define OPisg_word		C8h
#define OPisg_byte		C9h
#define OPisl_word		CAh
#define OPisl_byte		CBh
#define OPist_word		CCh
#define OPist_byte		CDh
#define OPisp_word		CEh
#define OPisp_byte		CFh
#define OPiagi_word	D0h
#define OPiagi_byte	D1h
#define OPiali_word	D2h
#define OPiali_byte	D3h
#define OPiati_word	D4h
#define OPiati_byte	D5h
#define OPiapi_word	D6h
#define OPiapi_byte	D7h
#define OPisgi_word	D8h
#define OPisgi_byte	D9h
#define OPisli_word	DAh
#define OPisli_byte	DBh
#define OPisti_word	DCh
#define OPisti_byte	DDh
#define OPispi_word	DEh
#define OPispi_byte	DFh
#define OPdag_word		E0h
#define OPdag_byte		E1h
#define OPdal_word		E2h
#define OPdal_byte		E3h
#define OPdat_word		E4h
#define OPdat_byte		E5h
#define OPdap_word		E6h
#define OPdap_byte		E7h
#define OPdsg_word		E8h
#define OPdsg_byte		E9h
#define OPdsl_word		EAh
#define OPdsl_byte		EBh
#define OPdst_word		ECh
#define OPdst_byte		EDh
#define OPdsp_word		EEh
#define OPdsp_byte		EFh
#define OPdagi_word	F0h
#define OPdagi_byte	F1h
#define OPdali_word	F2h
#define OPdali_byte	F3h
#define OPdati_word	F4h
#define OPdati_byte	F5h
#define OPdapi_word	F6h
#define OPdapi_byte	F7h
#define OPdsgi_word	F8h
#define OPdsgi_byte	F9h
#define OPdsli_word	FAh
#define OPdsli_byte	FBh
#define OPdsti_word	FCh
#define OPdsti_byte	FDh
#define OPdspi_word	FEh
#define OPdspi_byte	FFh

#endif
