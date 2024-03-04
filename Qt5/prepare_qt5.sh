#!/bin/bash

OLD_PWD="${PWD}"
BUILD_DIR="${PWD}/.build"
DOWNLOAD_DIR="${PWD}/.cache"
QT_SRC_FILE_URL="https://mirrors.tuna.tsinghua.edu.cn/qt/official_releases/qt/5.15/5.15.12/single/qt-everywhere-opensource-src-5.15.12.tar.xz"
QT_SRC_FILE="qt-everywhere-opensource-src.tar.xz"
QT_SRC_MD5SUM="3fb1cd4f763f5d50d491508b7b99fb77 qt-everywhere-opensource-src.tar.xz"

OUTPUT_DIR="${PWD}/lib"

function fetch_qt_source {
    curl --output-dir "${DOWNLOAD_DIR}" \
         --output    "${QT_SRC_FILE}" \
         -C - \
         -L ${QT_SRC_FILE_URL}
}

function ensure_download_dir {
    if [[ ! -d "${DOWNLOAD_DIR}" ]]; then
        mkdir -p ${DOWNLOAD_DIR}
    fi
}

function checksum_src_file {
    local chksum
    pushd ${DOWNLOAD_DIR}
    chksum=$(echo "${QT_SRC_MD5SUM}" | md5sum -c --status)
    popd

    return $chksum
}

ensure_download_dir

if [[ -f "${DOWNLOAD_DIR}/${QT_SRC_FILE}" ]]; then
    if [[ ! $(checksum_src_file) ]]; then
	echo "${QT_SRC_FILE} is not complete, fetching..."
        fetch_qt_source
    fi
else
    echo "${QT_SRC_FILE} is not available, fetching..."
    fetch_qt_source
fi

mkdir -p ${BUILD_DIR}
if [[ ! -f "${BUILD_DIR}/extracted" ]]; then
   echo "Extracting ${QT_SRC_FILE}"
   tar xf "${DOWNLOAD_DIR}/${QT_SRC_FILE}" -C "${BUILD_DIR}" --one-top-level="${BUILD_DIR}"
   touch "${BUILD_DIR}/extracted"
fi

QT_SRC_DIR=$(ls -d .build/qt-everywhere-src-5.*)
QT_BUILD_DIR="${QT_SRC_DIR}/../qt-build/"

mkdir -p "$QT_BUILD_DIR"
cd "${QT_BUILD_DIR}"

${OLD_PWD}/${QT_SRC_DIR}/configure -prefix ${OUTPUT_DIR} -opensource -confirm-license
make -j2
make install
