cp syntax.c case16.out
chmod +w case16.out
$CPROTO -a case16.out
