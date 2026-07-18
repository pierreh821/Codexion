#!/bin/bash

# Couleurs pour l'affichage
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

# Configuration de Valgrind
# --error-exitcode=42 force valgrind à renvoyer 42 si une fuite ou une erreur de mémoire est détectée
VALGRIND="valgrind --track-origins=yes --leak-check=full --error-exitcode=42 --quiet"
EXEC="./codexion"

clear
echo -e "${BLUE}=== RECOMPILATION DU PROJET ===${RESET}"
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}La compilation a échoué !${RESET}"
    exit 1
fi

TOTAL_TESTS=0
FAILED_TESTS=0

# Fonction utilitaire pour exécuter un test
run_test() {
    local desc="$1"
    shift
    local args="$@"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${YELLOW}[Test $TOTAL_TESTS] $desc${RESET}"
    echo "Commande: $EXEC $args"

    # Exécution avec Valgrind
    $VALGRIND $EXEC $args > /dev/null 2>&1
    local ret=$?

    if [ $ret -eq 42 ]; then
        echo -e "${RED}➜ RÉSULTAT : ÉCHEC (Erreur mémoire ou Fuite détectée par Valgrind)${RESET}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [ $ret -eq 139 ]; then
        echo -e "${RED}➜ RÉSULTAT : CRASH (Segmentation Fault)${RESET}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "${GREEN}➜ RÉSULTAT : SUCCÈS (Pas de fuite / Pas de crash)${RESET}"
    fi
    echo "----------------------------------------------------"
}

echo -e "\n${BLUE}=== DÉBUT DE LA BATTERIE DE TESTS ===${RESET}\n"

# ==============================================================================
# 1. ERREURS DE PARSING & ARGUMENTS INVALIDES
# ==============================================================================
run_test "Pas d'arguments"
run_test "Arguments insuffisants" 20 5000 2
run_test "Arguments non numériques" 20 abc 2 1 100 2 10 fifo
run_test "Arguments négatifs" -20 5000 2 1 100 2 10 fifo
run_test "Stratégie de scheduling invalide" 20 5000 2 1 100 2 10 lifo
run_test "Chaîne vide en stratégie" 20 5000 2 1 100 2 10 ""

# ==============================================================================
# 2. VALEURS LIMITES (OVERFLOWS & BOUNDS)
# ==============================================================================
run_test "Dépassement INT_MAX (Overflow)" 2147483648 5000 2 1 100 2 10 fifo
run_test "Nombre démesurément grand" 9999999999999999 5000 2 1 100 2 10 fifo

# ==============================================================================
# 3. CAS LIMITES LOGIQUES (EDGE CASES)
# ==============================================================================
run_test "Zéro codeur" 0 5000 2 1 100 2 10 fifo
run_test "Un seul codeur" 1 5000 2 1 100 2 10 fifo
run_test "Zéro dongle / ressources" 20 5000 0 1 100 2 10 fifo

# ==============================================================================
# 4. SCÉNARIOS DE SIMULATION (COMPLETION VS BURNOUT)
# ==============================================================================
# Note: Adapte les valeurs ci-dessous selon les règles exactes de ton sujet
run_test "Simulation standard (Succès)" 20 5000 2 1 100 2 10 fifo

# Exemple de valeurs provoquant un Burnout immédiat (ex: temps de mort très bas)
run_test "Scénario Burnout / Mort d'un codeur" 5 10 2 1 500 2 10 fifo

# Un grand nombre de threads pour stresser les mutex/conditions
run_test "Stress-test : Grand nombre de threads (50 codeurs)" 50 2000 5 2 50 5 5 fifo

# ==============================================================================
# BILAN DES TESTS
# ==============================================================================
echo -e "\n${BLUE}=== BILAN DES TESTS ===${RESET}"
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}Félicitations ! $TOTAL_TESTS/$TOTAL_TESTS tests passés avec succès (0 fuites, 0 crashs).${RESET}"
else
    echo -e "${RED}Attention : $FAILED_TESTS/$TOTAL_TESTS tests ont échoué.${RESET}"
    exit 1
fi
