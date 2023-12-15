LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://Tegra_Software_License_Agreement-Tegra-Linux.txt;md5=2c3e3a31b382cdbe8da31b1577fad03f \
                    file://nv_tegra/LICENSE.brcm_patchram_plus;md5=38fb07f0dacf4830bc57f40a0fb7532e"

SRC_URI = "\
    ${L4T_URI_BASE}/${L4T_BSP_PREFIX}_Linux_R${L4T_VERSION}_aarch64.tbz2 \
"

SRC_URI[sha256sum] = "50a2dc879711be310adc1844c5426e4aea6996949ba3e21e72f19ea09041fe96"

inherit l4t_bsp dos2unix

# Override convert_crlf_to_lf task to only convert "*.dts*" files
do_convert_crlf_to_lf () {
	find ${S} -type f  -name "*.dts*" -exec dos2unix {} \;
}


S = "${L4T_BSP_SHARED_SOURCE_DIR}"
B = "${WORKDIR}/build"

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

COMPATIBLE_MACHINE = "tegra"

do_preconfigure() {
    :
}