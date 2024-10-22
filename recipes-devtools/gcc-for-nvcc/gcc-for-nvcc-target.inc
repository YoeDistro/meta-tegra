GCCMULTILIB = "--enable-multilib"
require gcc-for-nvcc-configure-common.inc

DEPENDS += "virtual/${TARGET_PREFIX}cuda-gcc"

EXTRA_OECONF_PATHS = "\
    --with-build-sysroot=${STAGING_DIR_TARGET} \
"

EXTRA_OECONF:append:linuxstdbase = " --enable-clocale=gnu"

# Configure gcc running on the target to default to an architecture which will
# be compatible with that of gcc-runtime (which is cross compiled to be target
# specific). For example, for ARM, ARMv6+ adds atomic instructions that may
# affect the ABI in the gcc-runtime libs. Since we can't rely on gcc on the
# target to always be passed -march etc, its built-in default needs to be safe.

ARMFPARCHEXT ?= ""

EXTRA_OECONF:append:armv6:class-target = " --with-arch=armv6${ARMFPARCHEXT}"
EXTRA_OECONF:append:armv7a:class-target = " --with-arch=armv7-a${ARMFPARCHEXT}"
EXTRA_OECONF:append:armv7ve:class-target = " --with-arch=armv7ve${ARMFPARCHEXT}"
EXTRA_OECONF:append:arc:class-target = " --with-cpu=${TUNE_PKGARCH}"
EXTRA_OECONF:append:x86-64:class-target = " --with-arch=native"

# libcc1 requres gcc_cv_objdump when cross build, but gcc_cv_objdump is
# set in subdir gcc, so subdir libcc1 can't use it, export it here to
# fix the problem.
export gcc_cv_objdump = "${TARGET_PREFIX}objdump"

EXTRA_OECONF_GCC_FLOAT = "${@get_gcc_float_setting(bb, d)}"

PACKAGES = "\
    ${PN} ${PN}-plugins ${PN}-symlinks \
    g++-for-nvcc g++-for-nvcc-symlinks \
    cpp-for-nvcc cpp-for-nvcc-symlinks \
    gcov-for-nvcc gcov-for-nvcc-symlinks \
    ${PN}-doc \
    ${PN}-dev \
    ${PN}-dbg \
"

FILES:${PN} = "\
    ${bindir}/${TARGET_PREFIX}gcc* \
    ${bindir}/${TARGET_PREFIX}lto* \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/collect2* \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/g++-mapper-server \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/lto* \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/lib*${SOLIBS} \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/liblto*${SOLIBSDEV} \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/*.o \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/specs \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/lib*${SOLIBS} \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/include \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/include-fixed \
"
INSANE_SKIP:${PN} += "dev-so"
RRECOMMENDS:${PN} += "\
    libssp \
    libssp-dev \
"
RDEPENDS:${PN} += "cpp"

FILES:${PN}-dev = "\
    ${gcclibdir}/${TARGET_SYS}/${BINV}/lib*${SOLIBSDEV} \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/lib*${SOLIBSDEV} \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/plugin/include/ \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/plugin/gengtype \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/plugin/gtype.state \
"
FILES:${PN}-symlinks = "\
    ${bindir}/cc-${BINV} \
    ${bindir}/gcc-${BINV} \
    ${bindir}/gccbug-${BINV} \
"

FILES:${PN}-plugins = "\
    ${gcclibdir}/${TARGET_SYS}/${BINV}/plugin \
"
ALLOW_EMPTY:${PN}-plugins = "1"

FILES:g77 = "\
    ${bindir}/${TARGET_PREFIX}g77 \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/f771 \
"
FILES:g77-symlinks = "\
    ${bindir}/g77 \
    ${bindir}/f77 \
"
RRECOMMENDS:g77 = "\
    libg2c \
    libg2c-dev \
"

FILES:gfortran = "\
    ${bindir}/${TARGET_PREFIX}gfortran \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/f951 \
"
RRECOMMENDS:gfortran = "\
    libquadmath \
    libquadmath-dev \
"
FILES:gfortran-symlinks = "\
    ${bindir}/gfortran \
    ${bindir}/f95"

FILES:cpp-for-nvcc = "\
    ${bindir}/${TARGET_PREFIX}cpp* \
    ${base_libdir}/gcc/${TARGET_SYS}/${BINV}/cpp \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/cc1"
FILES:cpp-for-nvcc-symlinks = "${bindir}/cpp-${BINV}"

FILES:gcov-for-nvcc = "${bindir}/${TARGET_PREFIX}gcov-${BINV} \
    ${bindir}/${TARGET_PREFIX}gcov-tool-${BINV} \
    ${bindir}/${TARGET_PREFIX}gcov-dump-${BINV} \
"
FILES:gcov-for-nvcc-symlinks = "${bindir}/gcov-${BINV} \
    ${bindir}/gcov-tool-${BINV} \
"

FILES:g++-for-nvcc = "\
    ${bindir}/${TARGET_PREFIX}g++* \
    ${libexecdir}/gcc/${TARGET_SYS}/${BINV}/cc1plus \
"
FILES:g++-for-nvcc-symlinks = "\
    ${bindir}/c++-${BINV} \
    ${bindir}/g++-${BINV} \
"
RRECOMMENDS:g++-for-nvcc = "\
    libstdc++ \
    libstdc++-dev \
    libatomic \
    libatomic-dev \
"

FILES:${PN}-doc = "\
    ${infodir} \
    ${mandir} \
    ${gcclibdir}/${TARGET_SYS}/${BINV}/include/README \
"

do_compile () {
    remove_sysroot_paths_from_configargs '/'
    remove_sysroot_paths_from_checksum_options '${STAGING_DIR_TARGET}' '/'

	oe_runmake all-host
}

do_install () {
	oe_runmake 'DESTDIR=${D}' install-host

	# Add unwind.h, it comes from libgcc which we don't want to build again
	install ${STAGING_LIBDIR_NATIVE}/${TARGET_SYS}/gcc/${TARGET_SYS}/${BINV}/include/unwind.h ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/include/

	# Info dir listing isn't interesting at this point so remove it if it exists.
	if [ -e "${D}${infodir}/dir" ]; then
		rm -f ${D}${infodir}/dir
	fi

	# Cleanup some of the ${libdir}{,exec}/gcc stuff ...
	rm -r ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/install-tools
	rm -r ${D}${libexecdir}/gcc/${TARGET_SYS}/${BINV}/install-tools
	rm -rf ${D}${libexecdir}/gcc/${TARGET_SYS}/${BINV}/*.la
	rmdir ${D}${includedir}
	rm -rf ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/finclude

	# Hack around specs file assumptions
	test -f ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/specs && sed -i -e '/^*cross_compile:$/ { n; s/1/0/; }' ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/specs

	# Cleanup manpages..
	rm -rf ${D}${mandir}/man7

	# Don't package details about the build host
	rm -f ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/plugin/include/auto-build.h
	rm -f ${D}${libdir}/gcc/${TARGET_SYS}/${BINV}/plugin/include/bconfig.h

	cd ${D}${bindir}

	# We care about g++ not c++
	rm -f *c++*

	# Not sure why we end up with these but we don't want them...
	rm -f ${TARGET_PREFIX}${TARGET_PREFIX}*

	ln -sf ${TARGET_PREFIX}g++-${BINV} g++-${BINV}
	ln -sf ${TARGET_PREFIX}gcc-${BINV} gcc-${BINV}
	ln -sf ${TARGET_PREFIX}cpp-${BINV} cpp-${BINV}
	ln -sf ${TARGET_PREFIX}gcov-${BINV} gcov-${BINV}
	ln -sf ${TARGET_PREFIX}gcov-tool-${BINV} gcov-tool-${BINV}
	ln -sf g++-${BINV} c++-${BINV}
	ln -sf gcc-${BINV} cc-${BINV}
	chown -R root:root ${D}

	cleanup_installed_include_fixed
}

# Installing /usr/lib/gcc/* means we'd have two copies, one from gcc-cross
# and one from here. These can confuse gcc cross where includes use #include_next
# and builds track file dependencies (e.g. perl and its makedepends code).
# For determinism we don't install this ever and rely on the copy from gcc-cross.
# [YOCTO #7287]
SYSROOT_DIRS_IGNORE += "${libdir}/gcc"