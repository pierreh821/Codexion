#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RESET='\033[0m'

VALGRIND="valgrind --track-origins=yes --leak-check=full --error-exitcode=42 --quiet"
EXEC="./codexion"
LOG_DIR="./log"
DEFAULT_TIMEOUT=8

rm -rf "$LOG_DIR"
mkdir -p "$LOG_DIR"

clear
echo -e "${BLUE}=== RECOMPILATION DU PROJET ===${RESET}"
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}La compilation a échoué !${RESET}"
    exit 1
fi

TOTAL_TESTS=0
FAILED_TESTS=0

slugify() {
    echo "$1" | tr '[:upper:]' '[:lower:]' | sed 's/[^a-z0-9]/_/g' | sed 's/__*/_/g' | sed 's/^_//;s/_$//'
}

# run_test "description" [timeout_secondes] -- args...
run_test() {
    local desc="$1"
    local test_timeout="$2"
    shift 2
    local args="$@"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    local slug=$(slugify "$desc")
    local logfile="$LOG_DIR/test_$(printf '%02d' $TOTAL_TESTS)_${slug}.log"

    echo -e "${YELLOW}[Test $TOTAL_TESTS] $desc${RESET}"
    echo "Commande: $EXEC $args (timeout ${test_timeout}s)"

    {
        echo "=== $desc ==="
        echo "Commande: $EXEC $args"
        echo "Timeout: ${test_timeout}s"
        echo "---"
    } > "$logfile"

    timeout "$test_timeout" $VALGRIND $EXEC $args >> "$logfile" 2>&1
    local ret=$?

    echo "---" >> "$logfile"
    echo "Code de retour: $ret" >> "$logfile"

    if [ $ret -eq 42 ]; then
        echo -e "${RED}➜ ÉCHEC : erreur mémoire / fuite (voir $logfile)${RESET}"
        echo "VERDICT: FUITE/ERREUR MEMOIRE" >> "$logfile"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [ $ret -eq 124 ]; then
        echo -e "${RED}➜ ÉCHEC : le programme ne s'est pas arrêté seul (timeout, voir $logfile)${RESET}"
        echo "VERDICT: HANG / TIMEOUT" >> "$logfile"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [ $ret -eq 139 ]; then
        echo -e "${RED}➜ ÉCHEC : segmentation fault (voir $logfile)${RESET}"
        echo "VERDICT: CRASH" >> "$logfile"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "${GREEN}➜ SUCCÈS${RESET}"
        echo "VERDICT: OK" >> "$logfile"
    fi
    echo "----------------------------------------------------"
}

# run_repeated_test "description" nb_repetitions timeout_secondes -- args...
# Utile pour les bugs de timing (race conditions) qui ne se déclenchent
# qu'une fois de temps en temps - un seul passage ne prouve rien.
run_repeated_test() {
    local desc="$1"
    local count="$2"
    local test_timeout="$3"
    shift 3
    local args="$@"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    local slug=$(slugify "$desc")
    local subdir="$LOG_DIR/test_$(printf '%02d' $TOTAL_TESTS)_${slug}"
    mkdir -p "$subdir"

    echo -e "${YELLOW}[Test $TOTAL_TESTS] $desc (x$count répétitions)${RESET}"
    echo "Commande: $EXEC $args (timeout ${test_timeout}s par run)"

    local run_failed=0
    for i in $(seq 1 "$count"); do
        local logfile="$subdir/run_$(printf '%02d' $i).log"
        {
            echo "=== $desc (run $i/$count) ==="
            echo "Commande: $EXEC $args"
            echo "---"
        } > "$logfile"

        timeout "$test_timeout" $VALGRIND $EXEC $args >> "$logfile" 2>&1
        local ret=$?
        echo "---" >> "$logfile"
        echo "Code de retour: $ret" >> "$logfile"

        if [ $ret -eq 42 ] || [ $ret -eq 124 ] || [ $ret -eq 139 ]; then
            run_failed=1
            echo "VERDICT: ECHEC (code $ret)" >> "$logfile"
            echo -e "${RED}  run $i/$count : ÉCHEC (code $ret)${RESET}"
        else
            echo "VERDICT: OK" >> "$logfile"
        fi
    done

    if [ $run_failed -eq 1 ]; then
        echo -e "${RED}➜ ÉCHEC : au moins une exécution sur $count a échoué (voir $subdir/)${RESET}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "${GREEN}➜ SUCCÈS : $count/$count exécutions propres${RESET}"
    fi
    echo "----------------------------------------------------"
}

echo -e "\n${BLUE}=== DÉBUT DE LA BATTERIE DE TESTS ===${RESET}\n"

# ==============================================================================
# 1. ERREURS DE PARSING & ARGUMENTS INVALIDES
# ==============================================================================
run_test "Pas d'arguments" $DEFAULT_TIMEOUT
run_test "Arguments insuffisants" $DEFAULT_TIMEOUT 20 5000 2
run_test "Trop d'arguments" $DEFAULT_TIMEOUT 20 5000 2 1 100 2 10 fifo extra
run_test "Arguments non numériques" $DEFAULT_TIMEOUT 20 abc 2 1 100 2 10 fifo
run_test "Arguments négatifs" $DEFAULT_TIMEOUT -20 5000 2 1 100 2 10 fifo
run_test "Nombre à virgule flottante" $DEFAULT_TIMEOUT 20 5000.5 2 1 100 2 10 fifo
run_test "Nombre avec signe plus explicite" $DEFAULT_TIMEOUT +20 5000 2 1 100 2 10 fifo
run_test "Espace au sein d'un argument numérique" $DEFAULT_TIMEOUT " 20" 5000 2 1 100 2 10 fifo
run_test "Stratégie de scheduling invalide" $DEFAULT_TIMEOUT 20 5000 2 1 100 2 10 lifo
run_test "Stratégie en majuscules (doit être rejetée)" $DEFAULT_TIMEOUT 20 5000 2 1 100 2 10 FIFO
run_test "Chaîne vide en stratégie" $DEFAULT_TIMEOUT 20 5000 2 1 100 2 10 ""

# ==============================================================================
# 2. VALEURS LIMITES (OVERFLOWS & BOUNDS)
# ==============================================================================
run_test "Dépassement INT_MAX (Overflow)" $DEFAULT_TIMEOUT 2147483648 5000 2 1 100 2 10 fifo
run_test "Nombre démesurément grand" $DEFAULT_TIMEOUT 9999999999999999 5000 2 1 100 2 10 fifo
run_test "number_of_coders juste au-dessus de la limite" $DEFAULT_TIMEOUT 1025 5000 2 1 100 2 10 fifo
run_test "number_of_coders à la limite exacte haute" 15 1024 5000 2 1 100 2 10 fifo

# ==============================================================================
# 3. CAS LIMITES LOGIQUES (EDGE CASES)
# ==============================================================================
run_test "Zéro codeur" $DEFAULT_TIMEOUT 0 5000 2 1 100 2 10 fifo
run_test "Un seul codeur (dongle unique)" 20 1 5000 2 1 100 20 10 fifo
run_test "Deux codeurs (plus petit anneau à 2 dongles distincts)" 10 2 5000 2 1 100 5 10 fifo
run_test "time_to_compile = 0" $DEFAULT_TIMEOUT 20 5000 0 1 100 2 10 fifo
run_test "time_to_debug = 0" $DEFAULT_TIMEOUT 20 5000 2 0 100 2 10 fifo
run_test "time_to_refactor = 0" $DEFAULT_TIMEOUT 20 5000 2 1 0 2 10 fifo
run_test "number_of_compiles_required = 0" $DEFAULT_TIMEOUT 20 5000 2 1 100 0 10 fifo
run_test "dongle_cooldown = 0" $DEFAULT_TIMEOUT 20 5000 2 1 100 2 0 fifo
run_test "dongle_cooldown proche du temps de compile (forte contention)" 15 10 5000 5 1 100 5 10 fifo

# ==============================================================================
# 4. SCÉNARIOS DE SIMULATION (COMPLETION VS BURNOUT)
# ==============================================================================
run_test "Simulation standard (doit se terminer seule, marge large)" 10 20 5000 2 1 100 2 10 fifo
run_test "Simulation standard EDF (doit se terminer seule)" 10 20 5000 2 1 100 2 10 edf
run_test "Scénario Burnout garanti (deadline très courte)" $DEFAULT_TIMEOUT 5 10 2 1 500 2 10 fifo
run_test "Scénario Burnout garanti EDF" $DEFAULT_TIMEOUT 5 10 2 1 500 2 10 edf

# ==============================================================================
# 5. STRESS TESTS (concurrence, mutex, conditions)
# ==============================================================================
run_test "Stress : 50 codeurs, FIFO" 15 50 2000 5 2 50 5 5 fifo
run_test "Stress : 50 codeurs, EDF" 15 50 2000 5 2 50 5 5 edf
run_test "Stress : 200 codeurs" 20 200 2000 5 2 50 5 5 fifo

# Régression directe du bug de course au démarrage (create_team / create_monitor).
# Un seul passage ne suffit pas à le détecter, d'où les répétitions.
run_repeated_test "Régression : race au démarrage (50 codeurs)" 20 15 50 2000 5 2 50 5 5 fifo
run_repeated_test "Régression : race au démarrage EDF (50 codeurs)" 20 15 50 2000 5 2 50 5 5 edf

# ==============================================================================
# BILAN DES TESTS
# ==============================================================================
echo -e "\n${BLUE}=== BILAN DES TESTS ===${RESET}"
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}Félicitations ! $TOTAL_TESTS/$TOTAL_TESTS tests passés avec succès (0 fuites, 0 crashs, 0 hangs).${RESET}"
    echo -e "Logs disponibles dans ${LOG_DIR}/"
else
    echo -e "${RED}Attention : $FAILED_TESTS/$TOTAL_TESTS tests ont échoué.${RESET}"
    echo -e "Consulte les logs dans ${LOG_DIR}/ pour le détail."
    exit 1
fi
