CREATE MEMORY TABLE USERTABLE(ID NUMBER NOT NULL,NAME NVARCHAR2(50),AGE INTEGER,BIRTH DATE); 

CREATE SEQUENCE USERTABLE_ID_SEQ 

 

INSERT INTO USERTABLE VALUES(USERTABLE_ID_SEQ.NEXTVAL,'フィナルナユーザ１',28, to_date('2006/02/21 15:35:23','yyyy/mm/dd hh24:mi:ss')); 

INSERT INTO USERTABLE VALUES(USERTABLE_ID_SEQ.NEXTVAL,'フィナルナユーザ２',30, to_date('2006/02/21 15:35:23','yyyy/mm/dd hh24:mi:ss')); 

INSERT INTO USERTABLE VALUES(USERTABLE_ID_SEQ.NEXTVAL,'フィナルナユーザ３',33, to_date('2006/02/21 15:35:23','yyyy/mm/dd hh24:mi:ss')); 

INSERT INTO USERTABLE VALUES(USERTABLE_ID_SEQ.NEXTVAL,'フィナルナユーザ４',20, to_date('2006/02/21 15:35:23','yyyy/mm/dd hh24:mi:ss')); 

INSERT INTO USERTABLE VALUES(USERTABLE_ID_SEQ.NEXTVAL,'フィナルナユーザ５',24, to_date('2006/02/21 15:35:23','yyyy/mm/dd hh24:mi:ss')); 

 

 

-- Oracleはデータベース標準で、ncharによるUnicode(UTF16)がサポートされるまで、 

-- charに各国ごとのローカルエンコードマルチバイト文字を格納として対応。 

-- そのため、charとncharのJDBCの状況が古いOracle6等とOracle10とOracle11で対応が異なる上 

-- それぞれの文字列エンコードをシステムテーブル内に保持するという状況が生まれている。 

-- http://www.shift-the-oracle.com/config/multibyte-characterset.htmlこのあたりの解説に詳しい。 

-- Oracleの場合、JDBCのクライアント側では文字列を相互変換する必要があるため、どのデータ型に 

-- どんなエンコードで代入したかをきちんと把握しておく必要があり、文字列取得後の変換はクライアントのお仕事になる。 

-- OracleのドライバにはJREの各ベースバージョンごとに1.4、5、6の３つのバージョンがあり、Version6以降しかgetNStringは 

-- 使用できない。 

 

-- Oracleはミリ秒までをサポートしたTIMESTAMP型をOracle 9iで初導入。それまでは日付および時刻を保持する型として、 

-- DATE型をサポート。原型としては、NUMBER,CHAR,DATEの３つを標準型としてサポート。確かに帳簿管理だけならこれだけでよかったのかも。 

 

-- varcharは、データ型であって文字列型ではない。その後、ncharのUnicode文字列型のサポート拡充のため、Nvarchar2が登場した。 

-- しかし、元のUnicodeではないcharの可変長サポートのため、さらにvarchar2が登場する。 

 

-- SQL*Plus 以外の SQL 実行インターフェイスを使用している場合に SQL 文の終端にセミコロン(;) が付けてある。 

--動的 SQL の DDL や DML にはセミコロンは不要。 

--参考: 最後に入力する セミコロン(;) とスラッシュ(/) の意味 