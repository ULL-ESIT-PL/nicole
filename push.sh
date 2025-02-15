#!/bin/bash
# Script para crear un tag de versión o hacer un commit según el formato del argumento

# Verificar que se pasó al menos un argumento
if [ $# -eq 0 ]; then
    echo "Uso: $0 <version|mensaje de commit>"
    exit 1
fi

INPUT="$1"

# Verificar si el input corresponde a un número de versión (por ejemplo, 1.0.0)
if [[ $INPUT =~ ^[0-9]+\.[0-9]+\.[0-9]+(\.[0-9]+)*$ ]]; then
    VERSION="v$INPUT"
    echo "Creating tag $VERSION and pushing to remote repo"
    git tag "$VERSION"
    git push origin "$VERSION"
else
    INPUT="$@"
    echo "Commit with message: $INPUT"
    git add .
    git commit -m "$INPUT"
    git push
fi
