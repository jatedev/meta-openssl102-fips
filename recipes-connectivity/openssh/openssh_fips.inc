FILESEXTRAPATHS:prepend := "${THISDIR}/openssh:"
DEPENDS += " \
    openssl-fips \
    fipscheck \
"
RRECOMMENDS:${PN}-sshd:remove = "rng-tools"

SRC_URI += " \
    file://0001-openssh-8.6p1-fips.patch \
    file://0001-conditional-enable-fips-mode.patch \
    file://openssh-6.6p1-ctr-cavstest.patch \
    file://openssh-6.7p1-kdf-cavs.patch \
    file://0001-ssh-cavs-fix-the-double-free-error.patch \
    file://0001-ssh-cavs-set-kex-sessin_id-via-sshbuf_put.patch \
"

do_install:append() {
    install -d ${D}${libdir}/fipscheck
}

inherit qemu

pkg_postinst:append:${PN}-ssh () {
    if [ -n "$D" ]; then
        if ${@bb.utils.contains('MACHINE_FEATURES', 'qemu-usermode', 'true','false', d)}; then
            ${@qemu_run_binary(d, '$D', '${bindir}/fipshmac')} \
                -d $D${libdir}/fipscheck $D${bindir}/ssh.${BPN}
        else
            $INTERCEPT_DIR/postinst_intercept delay_to_first_boot ${PKG} mlprefix=${MLPREFIX}
        fi
    else
        ${bindir}/fipshmac -d ${libdir}/fipscheck ${bindir}/ssh.${BPN}
    fi
}

pkg_postinst:append:${PN}-sshd () {
    if [ -n "$D" ]; then
        if ${@bb.utils.contains('MACHINE_FEATURES', 'qemu-usermode', 'true','false', d)}; then
            ${@qemu_run_binary(d, '$D', '${bindir}/fipshmac')} \
                -d $D${libdir}/fipscheck $D${sbindir}/sshd
        else
            $INTERCEPT_DIR/postinst_intercept delay_to_first_boot ${PKG} mlprefix=${MLPREFIX}
        fi
    else
        ${bindir}/fipshmac -d ${libdir}/fipscheck ${sbindir}/sshd
    fi
}

PACKAGES =+ "${PN}-cavs"
SUMMARY:${PN}-cavs = "CAVS tests for FIPS validation"
FILES:${PN}-cavs = " \
    ${libexecdir}/ctr-cavstest \
    ${libexecdir}/ssh-cavs \
    ${libexecdir}/ssh-cavs_driver.pl"

FILES:${PN} += "${libdir}/fipscheck"
