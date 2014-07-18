#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/amlogic/vout/vinfo.h>
#include <linux/amlogic/vout/enc_clk_config.h>
#include <mach/register.h>
#include <mach/am_regs.h>
#include <mach/clock.h>

#define check_div() \
    if(div == -1)\
        return ;\
    switch(div){\
        case 1:\
            div = 0; break;\
        case 2:\
            div = 1; break;\
        case 4:\
            div = 2; break;\
        case 6:\
            div = 3; break;\
        case 12:\
            div = 4; break;\
        default:\
            break;\
    }

#define h_delay()       \
    do {                \
        int i = 1000;   \
        while(i--);     \
    }while(0)

static void set_hpll_clk_out(unsigned clk)
{
    printk("config HPLL\n");
#if MESON_CPU_TYPE >= MESON_CPU_TYPE_MESON8
    aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x69c88000);
    aml_write_reg32(P_HHI_VID_PLL_CNTL3, 0xca563823);
    aml_write_reg32(P_HHI_VID_PLL_CNTL4, 0x40238100);
    aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x00012286);
    aml_write_reg32(P_HHI_VID2_PLL_CNTL2, 0x430a800);       // internal LDO share with HPLL & VIID PLL
#endif
    switch(clk){
#ifdef CONFIG_ARCH_MESON8
        case 2970:
            aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x59c84e00);
            aml_write_reg32(P_HHI_VID_PLL_CNTL3, 0xce49c822);
            aml_write_reg32(P_HHI_VID_PLL_CNTL4, 0x4123b100);
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x00012385);

            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x6000043d);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x4000043d);
            printk("waiting HPLL lock\n");
            while(!(aml_read_reg32(P_HHI_VID_PLL_CNTL) & (1 << 31))) {
                ;
            }
            h_delay();
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x00016385);   // optimise HPLL VCO 2.97GHz performance
            break;
#endif
#ifdef CONFIG_ARCH_MESON8B
        case 2160:
            aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x59c80000);
            aml_write_reg32(P_HHI_VID_PLL_CNTL3, 0x0a563823);
            aml_write_reg32(P_HHI_VID_PLL_CNTL4, 0x0123b100);
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x12385);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x6001042d);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x4001042d);
            while(!(aml_read_reg32(P_HHI_VID_PLL_CNTL) & (1 << 31))) {
                ;
            }
            break;
#endif
        case 1488:
#ifdef CONFIG_ARCH_MESON8B
            aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x69c8ce00);
            aml_write_reg32(P_HHI_VID_PLL_CNTL4, 0x4023d100);
            aml_write_reg32(P_HHI_VID_PLL_CNTL3, 0x8a7ad023);
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x12286);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x6000043d);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x4000043d);
            while(!(aml_read_reg32(P_HHI_VID_PLL_CNTL) & (1 << 31))) {
                ;
            }
#endif
#ifdef CONFIG_ARCH_MESON8
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x6000043d);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x4000043d);
            printk("waiting HPLL lock[%d]\n", __LINE__);
            while(!(aml_read_reg32(P_HHI_VID_PLL_CNTL) & (1 << 31))) {
                ;
            }
            aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x69c8ce00);
#endif
#ifdef CONFIG_ARCH_MESON6
            WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x43e);
#endif
            break;
        case 1080:
#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
    aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x6000042d);
    aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x4000042d);
#endif
#ifdef CONFIG_ARCH_MESON6
            WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x42d);
#endif
            break;
        case 1066:
            WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x42a);
            break;
        case 1058:
            WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x422);
            break;
        case 1086:
            WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x43e);
            break;
        case 1296:
#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
            aml_write_reg32(P_HHI_VID_PLL_CNTL2, 0x59c88000);
            aml_write_reg32(P_HHI_VID_PLL_CNTL3, 0xca49b022);
            aml_write_reg32(P_HHI_VID_PLL_CNTL4, 0x0023b100);
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x00012385);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x600c0436);
            aml_write_reg32(P_HHI_VID_PLL_CNTL,  0x400c0436);
            aml_write_reg32(P_HHI_VID_PLL_CNTL5, 0x00016385);
#endif
            break;
        default:
            break;
    }
#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))

#endif
    printk("config HPLL done\n");
}

static void set_hpll_hdmi_od(unsigned div)
{
    switch(div){
        case 1:
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 0, 18, 2);
            break;
        case 2:
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 1, 18, 2);
            break;
        case 3:
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 1, 16, 2);
            break;
        case 4:
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 3, 18, 2);
            break;
        case 8:
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 1, 16, 2);
            WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 3, 18, 2);
            break;
        default:
            break;
    }
}

#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
static void set_hpll_lvds_od(unsigned div)
{
    switch(div) {
        case 1:
            aml_set_reg32_bits(P_HHI_VID_PLL_CNTL, 0, 16, 2);
            break;
        case 2:
            aml_set_reg32_bits(P_HHI_VID_PLL_CNTL, 1, 16, 2);
            break;
        case 4:
            aml_set_reg32_bits(P_HHI_VID_PLL_CNTL, 2, 16, 2);
            break;
        case 8:     // note: need test
            aml_set_reg32_bits(P_HHI_VID_PLL_CNTL, 3, 16, 2);
            break;
        default:
            break;
    }
}
#endif

// viu_channel_sel: 1 or 2
// viu_type_sel: 0: 0=ENCL, 1=ENCI, 2=ENCP, 3=ENCT.
int set_viu_path(unsigned viu_channel_sel, viu_type_e viu_type_sel)
{
    if((viu_channel_sel > 2) || (viu_channel_sel == 0))
        return -1;

#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
    printk("VPU_VIU_VENC_MUX_CTRL: 0x%x\n", aml_read_reg32(P_VPU_VIU_VENC_MUX_CTRL));
    if(viu_channel_sel == 1){
        aml_set_reg32_bits(P_VPU_VIU_VENC_MUX_CTRL, viu_type_sel, 0, 2);
        printk("viu chan = 1\n");
    }
    else{
        //viu_channel_sel ==2
        aml_set_reg32_bits(P_VPU_VIU_VENC_MUX_CTRL, viu_type_sel, 2, 2);
        printk("viu chan = 2\n");
    }
    printk("VPU_VIU_VENC_MUX_CTRL: 0x%x\n", aml_read_reg32(P_VPU_VIU_VENC_MUX_CTRL));
#endif
    return 0;
}

static void set_vid_pll_div(unsigned div)
{
#ifdef CONFIG_ARCH_MESON6
    // Gate disable
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 16, 1);
    switch(div){
        case 10:
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 4, 4, 3);
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 1, 8, 2);
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 1, 12, 3);
            break;
        case 5:
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 4, 4, 3);
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 8, 2);
            WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 12, 3);
            break;
        default:
            break;
    }
    // Soft Reset div_post/div_pre
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 0, 2);
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 1, 3, 1);
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 1, 7, 1);
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 3, 0, 2);
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 3, 1);
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 0, 7, 1);
    // Gate enable
    WRITE_CBUS_REG_BITS(HHI_VID_DIVIDER_CNTL, 1, 16, 1);
#endif
#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
    // Gate disable
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 16, 1);
    switch(div){
        case 10:
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 4, 4, 3);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 1, 8, 2);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 1, 12, 3);
            break;
        case 5:
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 4, 4, 3);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 8, 2);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 12, 3);
            break;
        case 6:
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 5, 4, 3);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 8, 2);
            aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 12, 3);
            break;
        default:
            break;
    }
    // Soft Reset div_post/div_pre
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 0, 2);
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 1, 3, 1);
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 1, 7, 1);
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 3, 0, 2);
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 3, 1);
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 0, 7, 1);
    // Gate enable
    aml_set_reg32_bits(P_HHI_VID_DIVIDER_CNTL, 1, 16, 1);
#endif
}

static void set_clk_final_div(unsigned div)
{
    if(div == 0)
        div = 1;
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_CNTL, 1, 19, 1);
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_CNTL, 0, 16, 3);
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_DIV, div-1, 0, 8);
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_CNTL, 7, 0, 3);
}

static void set_hdmi_tx_pixel_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_HDMI_CLK_CNTL, div, 16, 4);
}
static void set_encp_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_DIV, div, 24, 4);
}

static void set_enci_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_DIV, div, 28, 4);
}

static void set_enct_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VID_CLK_DIV, div, 20, 4);
}

static void set_encl_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VIID_CLK_DIV, div, 12, 4);
}

static void set_vdac0_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VIID_CLK_DIV, div, 28, 4);
}

static void set_vdac1_div(unsigned div)
{
    check_div();
    WRITE_CBUS_REG_BITS(HHI_VIID_CLK_DIV, div, 24, 4);
}

// mode hpll_clk_out hpll_hdmi_od viu_path viu_type vid_pll_div clk_final_div
// hdmi_tx_pixel_div unsigned encp_div unsigned enci_div unsigned enct_div unsigned ecnl_div;
static enc_clk_val_t setting_enc_clk_val[] = {
#ifdef CONFIG_ARCH_MESON6
    {VMODE_480I,       1080, 4, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480CVBS,    1080, 4, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480P,       1080, 4, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_576I,       1080, 4, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576CVBS,    1080, 4, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576P,       1080, 4, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_720P,       1488, 2, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080I,      1488, 2, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P,      1488, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P,      1488, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_720P_50HZ,  1488, 2, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080I_50HZ, 1488, 2, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P_50HZ, 1488, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P_24HZ, 1488, 2, 1, VIU_ENCP, 10, 2, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_VGA,  1066, 3, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  1},
    {VMODE_SVGA, 1058, 2, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  1},
    {VMODE_XGA, 1085, 1, 1, VIU_ENCP, 5, 1, 1, 1, -1, -1, -1,  1,  1},
#endif
#if MESON_CPU_TYPE >= MESON_CPU_TYPE_MESON8
#ifdef CONFIG_ARCH_MESON8B
    {VMODE_480I,       2160, 8, 1, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480CVBS,    1296, 4, 1, 1, VIU_ENCI,  6, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480P,       2160, 8, 1, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_576I,       2160, 8, 1, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576CVBS,    1296, 4, 1, 1, VIU_ENCI,  6, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576P,       2160, 8, 1, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
#endif
#ifdef CONFIG_ARCH_MESON8
    {VMODE_480I,       1080, 4, 1, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480CVBS,    1296, 4, 1, 1, VIU_ENCI,  6, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_480P,       1080, 4, 1, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_576I,       1080, 4, 1, 1, VIU_ENCI,  5, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576CVBS,    1296, 4, 1, 1, VIU_ENCI,  6, 4, 2,-1,  2, -1, -1,  2,  -1},
    {VMODE_576P,       1080, 4, 1, 1, VIU_ENCP,  5, 4, 2, 1, -1, -1, -1,  1,  -1},
#endif
    {VMODE_720P,       1488, 2, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080I,      1488, 2, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P,      1488, 1, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P,      1488, 1, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_720P_50HZ,  1488, 2, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080I_50HZ, 1488, 2, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P_50HZ, 1488, 1, 1, 1, VIU_ENCP, 10, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_1080P_24HZ, 1488, 2, 1, 1, VIU_ENCP, 10, 2, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_4K2K_30HZ,  2970, 1, 2, 1, VIU_ENCP,  5, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_4K2K_25HZ,  2970, 1, 2, 1, VIU_ENCP,  5, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_4K2K_24HZ,  2970, 1, 2, 1, VIU_ENCP,  5, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_4K2K_SMPTE, 2970, 1, 2, 1, VIU_ENCP,  5, 1, 1, 1, -1, -1, -1,  1,  -1},
    {VMODE_VGA,  1066, 3, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  1},
    {VMODE_SVGA, 1058, 2, 1, 1, VIU_ENCP, 10, 1, 2, 1, -1, -1, -1,  1,  1},
    {VMODE_XGA, 1085, 1, 1, 1, VIU_ENCP, 5, 1, 1, 1, -1, -1, -1,  1,  1},
#endif
};

void set_vmode_clk(vmode_t mode)
{
    enc_clk_val_t *p_enc = &setting_enc_clk_val[0];
    int i = sizeof(setting_enc_clk_val) / sizeof(enc_clk_val_t);
    int j = 0;
    
    printk("mode is: %d\n", mode);
    for (j = 0; j < i; j++){
        if(mode == p_enc[j].mode)
            break;
    }
    set_viu_path(p_enc[j].viu_path, p_enc[j].viu_type);
    set_hpll_clk_out(p_enc[j].hpll_clk_out);
#if ((defined CONFIG_ARCH_MESON8) || (defined CONFIG_ARCH_MESON8B))
    set_hpll_lvds_od(p_enc[j].hpll_lvds_od);
#endif
    set_hpll_hdmi_od(p_enc[j].hpll_hdmi_od);
    set_vid_pll_div(p_enc[j].vid_pll_div);
    set_clk_final_div(p_enc[j].clk_final_div);
    set_hdmi_tx_pixel_div(p_enc[j].hdmi_tx_pixel_div);
    set_encp_div(p_enc[j].encp_div);
    set_enci_div(p_enc[j].enci_div);
    set_enct_div(p_enc[j].enct_div);
    set_encl_div(p_enc[j].encl_div);
    set_vdac0_div(p_enc[j].vdac0_div);
    set_vdac1_div(p_enc[j].vdac1_div);
#ifdef CONFIG_ARCH_MESON6
    // If VCO outputs 1488, then we will reset it to exact 1485
    // please note, don't forget to re-config CNTL3/4
    if(((READ_CBUS_REG(HHI_VID_PLL_CNTL) & 0x7fff) == 0x43e)||((READ_CBUS_REG(HHI_VID_PLL_CNTL) & 0x7fff) == 0x21ef)) {
        WRITE_CBUS_REG_BITS(HHI_VID_PLL_CNTL, 0x21ef, 0, 14);
        WRITE_CBUS_REG(HHI_VID_PLL_CNTL3, 0x4b525012);
        WRITE_CBUS_REG(HHI_VID_PLL_CNTL4, 0x42000101);
    }
#endif
    
// For debug only
#if 0
    printk("hdmi debug tag\n%s\n%s[%d]\n", __FILE__, __FUNCTION__, __LINE__);
#define P(a)  printk("%s 0x%04x: 0x%08x\n", #a, a, READ_CBUS_REG(a))
    P(HHI_VID_PLL_CNTL);
    P(HHI_VID_DIVIDER_CNTL);
    P(HHI_VID_CLK_CNTL);
    P(HHI_VID_CLK_DIV);
    P(HHI_HDMI_CLK_CNTL);
    P(HHI_VIID_CLK_DIV);
#define PP(a) printk("%s(%d): %d MHz\n", #a, a, clk_util_clk_msr(a))
    PP(CTS_PWM_A_CLK        );
    PP(CTS_PWM_B_CLK        );
    PP(CTS_PWM_C_CLK        );
    PP(CTS_PWM_D_CLK        );
    PP(CTS_ETH_RX_TX        );
    PP(CTS_PCM_MCLK         );
    PP(CTS_PCM_SCLK         );
    PP(CTS_VDIN_MEAS_CLK    );
    PP(CTS_VDAC_CLK1        );
    PP(CTS_HDMI_TX_PIXEL_CLK);
    PP(CTS_MALI_CLK         );
    PP(CTS_SDHC_CLK1        );
    PP(CTS_SDHC_CLK0        );
    PP(CTS_AUDAC_CLKPI      );
    PP(CTS_A9_CLK           );
    PP(CTS_DDR_CLK          );
    PP(CTS_VDAC_CLK0        );
    PP(CTS_SAR_ADC_CLK      );
    PP(CTS_ENCI_CLK         );
    PP(SC_CLK_INT           );
    PP(USB_CLK_12MHZ        );
    PP(LVDS_FIFO_CLK        );
    PP(HDMI_CH3_TMDSCLK     );
    PP(MOD_ETH_CLK50_I      );
    PP(MOD_AUDIN_AMCLK_I    );
    PP(CTS_BTCLK27          );
    PP(CTS_HDMI_SYS_CLK     );
    PP(CTS_LED_PLL_CLK      );
    PP(CTS_VGHL_PLL_CLK     );
    PP(CTS_FEC_CLK_2        );
    PP(CTS_FEC_CLK_1        );
    PP(CTS_FEC_CLK_0        );
    PP(CTS_AMCLK            );
    PP(VID2_PLL_CLK         );
    PP(CTS_ETH_RMII         );
    PP(CTS_ENCT_CLK         );
    PP(CTS_ENCL_CLK         );
    PP(CTS_ENCP_CLK         );
    PP(CLK81                );
    PP(VID_PLL_CLK          );
    PP(AUD_PLL_CLK          );
    PP(MISC_PLL_CLK         );
    PP(DDR_PLL_CLK          );
    PP(SYS_PLL_CLK          );
    PP(AM_RING_OSC_CLK_OUT1 );
    PP(AM_RING_OSC_CLK_OUT0 );
#endif
}
 
