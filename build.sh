#!/bin/bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
BUILD_ORIGINAL=0
BUILD_CPP20=0
BUILD_TYPE="all"
VERBOSE=0

# Print usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Options:
    -o, --original      Build original beacon variant
    -c, --cpp20         Build C++20 beacon variant
    -a, --all           Build both variants (default)
    -t, --type TYPE     Build type: all, x64, x86 (default: all)
    -v, --verbose       Enable verbose output
    -h, --help          Show this help message

Examples:
    $0 --all --type x64              Build both variants for x64
    $0 --cpp20 --type x86            Build C++20 variant for x86
    $0 --original                    Build original variant with default settings

EOF
    exit 0
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--original)
            BUILD_ORIGINAL=1
            shift
            ;;
        -c|--cpp20)
            BUILD_CPP20=1
            shift
            ;;
        -a|--all)
            BUILD_ORIGINAL=1
            BUILD_CPP20=1
            shift
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

# Default: build both if none specified
if [ $BUILD_ORIGINAL -eq 0 ] && [ $BUILD_CPP20 -eq 0 ]; then
    BUILD_ORIGINAL=1
    BUILD_CPP20=1
fi

# Verify make is available
if ! command -v make &> /dev/null; then
    echo -e "${RED}Error: make is not installed${NC}"
    exit 1
fi

# Verify mingw-w64 is available
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo -e "${RED}Error: MinGW-w64 is not installed${NC}"
    exit 1
fi

echo -e "${GREEN}================================${NC}"
echo -e "${GREEN}Beacon Build Script${NC}"
echo -e "${GREEN}================================${NC}"
echo ""

# Build original beacon
if [ $BUILD_ORIGINAL -eq 1 ]; then
    echo -e "${YELLOW}Building Original Beacon (Build Type: $BUILD_TYPE)${NC}"
    
    if [ "$BUILD_TYPE" = "all" ]; then
        make all
    elif [ "$BUILD_TYPE" = "x64" ]; then
        make clean
        make pre
        make x64
    elif [ "$BUILD_TYPE" = "x86" ]; then
        make clean
        make pre
        make x86
    else
        echo -e "${RED}Unknown build type: $BUILD_TYPE${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Original beacon build completed${NC}"
    echo ""
fi

# Build C++20 beacon
if [ $BUILD_CPP20 -eq 1 ]; then
    echo -e "${YELLOW}Building C++20 Beacon (Build Type: $BUILD_TYPE)${NC}"
    
    if [ "$BUILD_TYPE" = "all" ]; then
        make all-cpp20
    elif [ "$BUILD_TYPE" = "x64" ]; then
        make clean-cpp20
        make pre-cpp20
        make x64-cpp20
    elif [ "$BUILD_TYPE" = "x86" ]; then
        make clean-cpp20
        make pre-cpp20
        make x86-cpp20
    else
        echo -e "${RED}Unknown build type: $BUILD_TYPE${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ C++20 beacon build completed${NC}"
    echo ""
fi

# Summary
echo -e "${GREEN}================================${NC}"
echo -e "${GREEN}Build Summary${NC}"
echo -e "${GREEN}================================${NC}"

if [ $BUILD_ORIGINAL -eq 1 ]; then
    echo -e "${GREEN}✓ Original Beacon${NC}"
    if [ -d "objects_http" ]; then
        echo "  - HTTP variant: $(ls objects_http/*.o 2>/dev/null | wc -l) objects"
    fi
    if [ -d "objects_smb" ]; then
        echo "  - SMB variant: $(ls objects_smb/*.o 2>/dev/null | wc -l) objects"
    fi
    if [ -d "objects_tcp" ]; then
        echo "  - TCP variant: $(ls objects_tcp/*.o 2>/dev/null | wc -l) objects"
    fi
fi

if [ $BUILD_CPP20 -eq 1 ]; then
    echo -e "${GREEN}✓ C++20 Beacon${NC}"
    if [ -d "objects_http_cpp20" ]; then
        echo "  - HTTP variant: $(ls objects_http_cpp20/*.o 2>/dev/null | wc -l) objects"
    fi
    if [ -d "objects_smb_cpp20" ]; then
        echo "  - SMB variant: $(ls objects_smb_cpp20/*.o 2>/dev/null | wc -l) objects"
    fi
    if [ -d "objects_tcp_cpp20" ]; then
        echo "  - TCP variant: $(ls objects_tcp_cpp20/*.o 2>/dev/null | wc -l) objects"
    fi
fi

echo ""
echo -e "${GREEN}Build completed successfully!${NC}"
