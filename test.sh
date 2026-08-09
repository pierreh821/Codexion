#!/bin/bash

# test_args.sh — focused on argument parsing, error handling, memory
# correctness, and thread-safety, using the official grading-scale
# reference cases as benchmarks.
#
# Three separate Valgrind tools are used (they cannot be combined):
#   - memcheck (--leak-check=full)  : leaks, invalid reads/writes, double free
#   - helgrind                      : data races, lock-ordering issues
#   - drd                           : data races, deadlocks (different heuristics than helgrind)
#
# Usage: ./test_args.sh

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
RESET='\033[0m'

MEMCHECK="valgrind --tool=memcheck --track-origins=yes --leak-check=full --error-exitcode=42 --quiet"
HELGRIND="valgrind --tool=helgrind --error-exitcode=42 --quiet"
DRD="valgrind --tool=drd --error-exitcode=42 --quiet"
EXEC="./codexion"
LOG_DIR="./log_args"
DEFAULT_TIMEOUT=8
# helgrind/drd instrument every memory access and every lock/unlock,
# which is far slower than memcheck — scale the timeout up so a slow
# tool pass isn't mistaken for a real hang.
THREAD_TOOL_MULTIPLIER=4

rm -rf "$LOG_DIR"
mkdir -p "$LOG_DIR"

clear
echo -e "${BLUE}=== RECOMPILATION ===${RESET}"
make re
if [ $? -ne 0 ]; then
	echo -e "${RED}Compilation failed.${RESET}"
	exit 1
fi

TOTAL=0
FAILED=0

slugify() {
	echo "$1" | tr '[:upper:]' '[:lower:]' | sed 's/[^a-z0-9]/_/g' | sed 's/__*/_/g' | sed 's/^_//;s/_$//'
}

# Runs one command under one valgrind tool, logs it, returns 0/1 (ok/fail).
# $1=logfile $2=timeout $3=tool_label $4=tool_cmd, remaining args (array-safe,
# via "$@") = the exact program arguments, whitespace/empty entries preserved.
run_under_tool() {
	local logfile="$1"
	local test_timeout="$2"
	local tool_label="$3"
	local tool_cmd="$4"
	shift 4
	local ret

	{
		echo "=== [$tool_label] ==="
		printf 'Commande: %s' "$EXEC"
		printf ' [%s]' "$@"
		echo
		echo "---"
	} >> "$logfile"

	# $tool_cmd is intentionally left unquoted: it is a fixed set of
	# valgrind flags meant to word-split. "$@" is NOT flattened: each
	# program argument is forwarded exactly as captured, so a leading
	# space, a trailing space, or an empty string reaches ./codexion
	# unmodified instead of being re-split by the shell.
	timeout "$test_timeout" $tool_cmd "$EXEC" "$@" >> "$logfile" 2>&1
	ret=$?
	echo "--- [$tool_label] code de retour: $ret ---" >> "$logfile"

	if [ $ret -eq 42 ]; then
		echo -e "${RED}    [$tool_label] ÉCHEC (erreur détectée, code 42)${RESET}"
		return 1
	elif [ $ret -eq 124 ]; then
		echo -e "${RED}    [$tool_label] ÉCHEC (timeout / hang)${RESET}"
		return 1
	elif [ $ret -eq 139 ]; then
		echo -e "${RED}    [$tool_label] ÉCHEC (segfault)${RESET}"
		return 1
	fi
	echo -e "${GREEN}    [$tool_label] OK (code $ret)${RESET}"
	return 0
}

# expect_reject "description" -- args...
# No threads are expected to be created on an invalid input (args are
# validated before create_team), so a single memcheck pass is enough
# here — it stays fast and still catches double free / invalid free on
# the error-handling path itself.
expect_reject() {
	local desc="$1"
	shift
	local -a prog_args=("$@")
	local ok=1

	TOTAL=$((TOTAL + 1))
	local slug=$(slugify "$desc")
	local logfile="$LOG_DIR/reject_$(printf '%02d' $TOTAL)_${slug}.log"
	echo "=== REJECT: $desc ===" > "$logfile"

	echo -e "${YELLOW}[Reject $TOTAL] $desc${RESET}"
	run_under_tool "$logfile" "$DEFAULT_TIMEOUT" "memcheck" "$MEMCHECK" "${prog_args[@]}"
	[ $? -ne 0 ] && ok=0

	# A silent exit code 0 on a supposedly-invalid input is suspicious:
	# it may mean the bad input was actually accepted instead of rejected.
	if [ $ok -eq 1 ] && grep -q "code de retour: 0" "$logfile"; then
		echo -e "${RED}    SUSPECT : code 0 — vérifie que l'entrée a bien été rejetée (voir $logfile)${RESET}"
		ok=0
	fi

	if [ $ok -eq 1 ]; then
		echo -e "${GREEN}➜ OK${RESET}"
	else
		echo -e "${RED}➜ ÉCHEC (voir $logfile)${RESET}"
		FAILED=$((FAILED + 1))
	fi
	echo "----------------------------------------------------"
}

# expect_run "description" timeout -- args...
# Real threaded execution: run once under EACH of the three tools.
# A failure in any one of them fails the test.
expect_run() {
	local desc="$1"
	local test_timeout="$2"
	shift 2
	local -a prog_args=("$@")
	local ok=1

	TOTAL=$((TOTAL + 1))
	local slug=$(slugify "$desc")
	local logfile="$LOG_DIR/run_$(printf '%02d' $TOTAL)_${slug}.log"
	echo "=== RUN: $desc ===" > "$logfile"

	echo -e "${CYAN}[Run $TOTAL] $desc${RESET}"

	run_under_tool "$logfile" "$test_timeout" "memcheck" "$MEMCHECK" "${prog_args[@]}"
	[ $? -ne 0 ] && ok=0

	run_under_tool "$logfile" "$((test_timeout * THREAD_TOOL_MULTIPLIER))" "helgrind" "$HELGRIND" "${prog_args[@]}"
	[ $? -ne 0 ] && ok=0

	run_under_tool "$logfile" "$((test_timeout * THREAD_TOOL_MULTIPLIER))" "drd" "$DRD" "${prog_args[@]}"
	[ $? -ne 0 ] && ok=0

	if [ $ok -eq 1 ]; then
		echo -e "${GREEN}➜ OK sur les 3 outils — relis $logfile pour vérifier burnout/complétion attendus${RESET}"
	else
		echo -e "${RED}➜ ÉCHEC sur au moins un outil (voir $logfile)${RESET}"
		FAILED=$((FAILED + 1))
	fi
	echo "----------------------------------------------------"
}

echo -e "\n${BLUE}=== 1. ARGUMENTS FACILES (comptage, format de base) ===${RESET}\n"

expect_reject "Aucun argument"
expect_reject "Un seul argument"                   5
expect_reject "Arguments manquants (5 sur 8)"       20 5000 200 200 200
expect_reject "Un argument de trop"                 20 5000 200 200 200 10 0 fifo extra
expect_reject "Arguments dans le mauvais ordre (scheduler en 1er)" fifo 20 5000 200 200 200 10 0

echo -e "\n${BLUE}=== 2. ARGUMENTS DIFFICILES (format numérique) ===${RESET}\n"

expect_reject "Nombre négatif"                      -20 5000 200 200 200 10 0 fifo
expect_reject "Zéro codeur"                         0 5000 200 200 200 10 0 fifo
expect_reject "Flottant"                            20 5000.5 200 200 200 10 0 fifo
expect_reject "Notation scientifique"                20 5e3 200 200 200 10 0 fifo
expect_reject "Hexadécimal"                          20 0x1388 200 200 200 10 0 fifo
expect_reject "Espace en tête d'un nombre"           " 20" 5000 200 200 200 10 0 fifo
expect_reject "Espace en fin d'un nombre"            "20 " 5000 200 200 200 10 0 fifo
expect_reject "Signe plus explicite"                 +20 5000 200 200 200 10 0 fifo
expect_reject "Chiffres avec séparateur"             20 5,000 200 200 200 10 0 fifo
expect_reject "Dépassement INT_MAX"                  2147483648 5000 200 200 200 10 0 fifo
expect_reject "Nombre absurdement long"               99999999999999999999 5000 200 200 200 10 0 fifo
expect_reject "Chaîne vide en argument numérique"    "" 5000 200 200 200 10 0 fifo

echo -e "\n${BLUE}=== 3. ARGUMENTS TROMPEURS ===${RESET}\n"

expect_reject "Nombre suivi de texte"                 20abc 5000 200 200 200 10 0 fifo
expect_reject "Texte suivi de nombre"                 abc20 5000 200 200 200 10 0 fifo
expect_reject "Nombre avec unité collée"              20ms 5000 200 200 200 10 0 fifo
expect_reject "Scheduler avec casse différente"       20 5000 200 200 200 10 0 FIFO
expect_reject "Scheduler avec espace"                 20 5000 200 200 200 10 0 " fifo"
expect_reject "Scheduler proche mais invalide"        20 5000 200 200 200 10 0 fifoo
expect_reject "Scheduler vide"                        20 5000 200 200 200 10 0 ""
expect_reject "Scheduler = lifo (pas encore autorisé sans recode)" 20 5000 200 200 200 10 0 lifo
expect_reject "number_of_compiles_required négatif"    20 5000 200 200 200 -1 0 fifo
expect_reject "dongle_cooldown négatif"                20 5000 200 200 200 10 -50 fifo
expect_reject "time_to_burnout négatif"                20 -5000 200 200 200 10 0 fifo

echo -e "\n${BLUE}=== 4. CAS QUI DOIVENT PASSER MAIS PEUVENT CASSER (bords légaux) ===${RESET}\n"

expect_run "number_of_compiles_required = 0 (arrêt immédiat)" $DEFAULT_TIMEOUT \
	5 5000 200 200 200 0 0 fifo
expect_run "dongle_cooldown = 0"                      $DEFAULT_TIMEOUT 5 5000 200 200 200 5 0 fifo
expect_run "Un seul codeur, forcément un seul dongle" $DEFAULT_TIMEOUT 1 5000 200 200 200 5 0 fifo
expect_run "Deux codeurs (plus petit anneau à 2 dongles distincts)" $DEFAULT_TIMEOUT \
	2 5000 200 200 200 5 0 fifo

echo -e "\n${BLUE}=== 5. CAS DE RÉFÉRENCE OFFICIELS DU BARÈME (Easy) ===${RESET}\n"
echo -e "${YELLOW}Lis les logs pour confirmer manuellement le comportement attendu (burnout au bon moment, etc).${RESET}\n"

expect_run "[Easy] 1 seul codeur -> DOIT burnout vers t=800" $DEFAULT_TIMEOUT \
	1 800 200 200 200 10 0 fifo
expect_run "[Easy] 5 codeurs, marge large fifo -> PAS de burnout, stop après 10 compiles chacun" 15 \
	5 2000 200 200 200 10 0 fifo
expect_run "[Easy] Idem en edf, 7 compiles chacun" 15 \
	5 2000 200 200 200 7 0 edf

echo -e "\n${BLUE}=== 6. CAS DE RÉFÉRENCE OFFICIELS DU BARÈME (Less easy) ===${RESET}\n"

expect_run "[Less easy] Infeasible par design -> DOIT burnout vers t=500, 'burned out' doit être la DERNIÈRE ligne" $DEFAULT_TIMEOUT \
	5 500 200 200 200 10 0 fifo

echo -e "\n${BLUE}=== 7. CAS DE RÉFÉRENCE OFFICIELS DU BARÈME (Medium) ===${RESET}\n"

expect_run "[Medium] Cooldown 400ms, marge large -> PAS de burnout, vérifier aucun 'has taken' <400ms après release" 15 \
	5 3000 200 200 200 10 400 fifo
expect_run "[Medium] Fort cooldown, fifo -> comparer avec la ligne suivante en edf" 20 \
	5 3000 200 200 200 10 800 fifo
expect_run "[Medium] Même paramètres en edf -> ordre d'octroi peut différer de fifo" 20 \
	5 3000 200 200 200 10 800 edf

echo -e "\n${BLUE}=== BILAN ===${RESET}"
if [ $FAILED -eq 0 ]; then
	echo -e "${GREEN}$TOTAL/$TOTAL tests passés (memcheck + helgrind + drd sur les scénarios threadés).${RESET}"
	echo -e "${YELLOW}Pense à relire manuellement les logs des sections 5-7 (./log_args/run_*) pour valider le comportement exact — un script ne peut pas juger seul si le burnout tombe au bon timestamp ou si l'ordre fifo/edf diverge correctement.${RESET}"
else
	echo -e "${RED}$FAILED/$TOTAL tests ont échoué. Consulte ./log_args/ pour le détail.${RESET}"
	exit 1
fi
