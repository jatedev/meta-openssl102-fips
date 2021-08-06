FILESEXTRAPATHS:prepend := "${LAYER_PATH_meta-openssl-one-zero-two-fips}/recipes-kernel/linux/files/:"
SRC_URI:append = " \
    file://crypto_fips.scc \
"
