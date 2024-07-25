Subversion hook redmine


http://d.hatena.ne.jp/sadaharu28/20100620/1277027057
#!/bin/sh
REPOS="$1"TXN="$2"
# Make sure that the log message contains some text.SVNLOOK=/usr/bin/svnlook 
#$SVNLOOK log -t "$TXN" "$REPOS" | \#   grep "[a-zA-Z0-9]" > /dev/null || exit 1
#参照用のチケット番号の確認
$SVNLOOK log -t "$TXN" "$REPOS" | \ 
grep -i -E "refs[ ]*[#]*[0-9]+" > /dev/null || \{ echo "No refs ticket number.(refs #)" 1>&2 && exit 1; }
#チケット進捗状態確認の関数 
getTicketRatio(){ 
mysql -s -u root redmine <<__REDMINE__select    
issue_statuses.default_done_ratiofrom    issues ,   issue_statuseswhere    issues.id = ${1}and issues.status_id = issue_statuses.id; __REDMINE__}
#参照用のチケット番号の取り出し
TICKET_NO=`$SVNLOOK log -t "$TXN" "$REPOS" | sed -e "s/.*refs[ ]\+[#]*\([0-9]\+\).*/\1/i" | grep -i -E "^[0-9]+$"`
if [ "NULL" == ${TICKET_NO:-NULL} ]; then 
  echo "No refs ticket number.(refs #)" 1>&2 
  exit 1else 
  #チケット進捗状態確認 
  TICKET_RATIO=`getTicketRatio ${TICKET_NO}`
#進捗状態が確認できない場合はコミット不可 
  if [ "NULL" == ${TICKET_RATIO:-NULL} ]; then 
    echo "Ticket #${TICKET_NO} is nothing." 1>&2 
    exit 1 
  fi  
fi
#完了用のチケット番号の取り出しTICKET_NO=`$SVNLOOK log -t "$TXN" "$REPOS" | sed -e "s/.*fixes[ ]\+[#]*\([0-9]\+\).*/\1/i" | grep -i -E "^[0-9]+$"`if [ "NULL" != ${TICKET_NO:-NULL} ]; then 
  #チケット進捗状態確認 
  TICKET_RATIO=`getTicketRatio ${TICKET_NO}`
#100%の時はコミット不可 
  if [ "NULL" == ${TICKET_RATIO:-NULL} ]; then 
    echo "Ticket #${TICKET_NO} is nothing." 1>&2 
    exit 1 
  elif [ 100 -eq ${TICKET_RATIO} ]; then 
    echo "Ticket #${TICKET_NO} is complite." 1>&2 
    exit 1 
  fifi
# Check that the author of this commit has the rights to perform# the commit on the files and directories being modified.#commit-access-control.pl "$REPOS" "$TXN" commit-access-control.cfg || exit 1
# All checks passed, so allow the commit.exit 0
From <http://d.hatena.ne.jp/sadaharu28/20100620/1277027057>