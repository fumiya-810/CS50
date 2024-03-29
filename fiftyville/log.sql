-- Keep a log of any SQL queries you execute as you solve the mystery.
-- 謎を解くときに実行した SQL クエリのログを保存します。

--テーブルの一覧を表示(Show list of tables)
.tables

-- 犯罪報告書のテーブルの列を表示, (Show column for crime_scene_reports)
.schema crime_scene_reports
-- CREATE TABLE crime_scene_reports (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     street TEXT,
--     description TEXT,
--     PRIMARY KEY(id)
-- );

-- 犯罪報告書の7月28日のハンフリー ストリートについて表示
select description from crime_scene_reports
where month = 7 and day = 28
and street = 'Humphrey Street';

-- CS50アヒルの盗難は、ハンフリーストリートのパン屋で午前10時15分に発生しました。
--  今日、当時その場にいた3人の証人に対してインタビューが行われたが、
--  彼らのインタビュー記録にはそれぞれパン屋について言及されていた。 |
-- | 16時36分にポイ捨てが行われました。 既知の目撃者はいない。

-- ベーカリーのセキュリティログを表示(Show column for bakery_security_logs)
.schema bakery_security_logs
-- CREATE TABLE bakery_security_logs (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     activity TEXT,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );


-- 7月28日10時代の出来事を表示(Displaying events from the 10th era)
select * from bakery_security_logs where month = 7 and day = 28 and hour = 10;
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 258 | 2023 | 7     | 28  | 10   | 8      | entrance | R3G7486       |
-- | 259 | 2023 | 7     | 28  | 10   | 14     | entrance | 13FNH73       |
-- | 260 | 2023 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2023 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2023 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2023 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2023 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2023 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
-- | 268 | 2023 | 7     | 28  | 10   | 35     | exit     | 1106N58       |
-- | 269 | 2023 | 7     | 28  | 10   | 42     | entrance | NRYN856       |
-- | 270 | 2023 | 7     | 28  | 10   | 44     | entrance | WD5M8I6       |
-- | 271 | 2023 | 7     | 28  | 10   | 55     | entrance | V47T75I       |
-- +-----+------+-------+-----+------+--------+----------+---------------+

-- license_plate(ナンバープレート)が気になったので関係がありそうなpeopleの列を表示
-- (I was curious about the license plate, so I displayed a column of people who seemed to be related.)
.schema people
-- CREATE TABLE people (
--     id INTEGER,
--     name TEXT,
--     phone_number TEXT,
--     passport_number INTEGER,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );



-- 聞き込み情報に手掛かりがないか調べた（I investigated whether there were any clues in the information I had heard.）
select transcript from interviews where month = 7 and day = 28;

--事件当日の記録からパン屋に関係のある証言を残す
-- | 盗難から10分も経たないうちに、泥棒がパン屋の駐車場にある車に乗り込み、走り去るのを目撃しました。
-- パン屋の駐車場の防犯映像がある場合は、その時間枠で駐車場から出た車を探すとよいでしょう。 |
-- | 泥棒の名前は知りませんが、見覚えのある人でした。 今朝早く、エマのパン屋に着く前に、
-- レゲット・ストリートのATMの前を歩いていたところ、そこで泥棒がお金を引き出しているのを見ました。 |
-- | 泥棒がパン屋から出ようとしたとき、誰かに電話をかけたところ、1分も経たずに会話をされました。
--  電話の中で、私は泥棒が明日フィフティビルから一番早い飛行機に乗る予定だと話しているのを聞きました。
--   次に、泥棒は電話の相手に航空券を購入するように頼みました。 |

-- 上記をまとめると以下のようになる
-- ・犯人は事件翌日の一番早いフィフティビル発の便の飛行機に乗った
-- ・犯人は事件当日の早朝にレゲットストリートのATMでお金を引き出していた。
-- ・犯人はパン屋を出るときに電話をした
-- ・犯人は事件発生から10分以内に店を出ていた。この時間帯に店を出たのは以下の8人である。
-- | 260 | 2023 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2023 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2023 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2023 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2023 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2023 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |

-- 事件発生から10分以内にパン屋を出た８人の情報を取得(Information on 8 people who left the bakery within 10 minutes of the incident was obtained.)
select * from people where license_plate in ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+



-- レゲットストリートのATMで取引をしたのは以下の通り（Transactions made at Leggett Street ATMs are as follows:）
select * from atm_transactions where month = 7 and day = 28 and atm_location = 'Leggett Street';
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2023 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2023 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2023 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2023 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 275 | 86363979       | 2023 | 7     | 28  | Leggett Street | deposit          | 10     |
-- | 288 | 25506511       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2023 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2023 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+

-- 事件当日に引き出していた人の銀行アカウント情報を表示
select * from bank_accounts where account_number in (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199);
-- +----------------+-----------+---------------+
-- | account_number | person_id | creation_year |
-- +----------------+-----------+---------------+
-- | 49610011       | 686048    | 2010          |
-- | 26013199       | 514354    | 2012          |
-- | 16153065       | 458378    | 2012          |
-- | 28296815       | 395717    | 2014          |
-- | 25506511       | 396669    | 2014          |
-- | 28500762       | 467400    | 2014          |
-- | 76054385       | 449774    | 2015          |
-- | 81061156       | 438727    | 2018          |
-- +----------------+-----------+---------------+

-- 事件現場から10分以内に立ち去った者と当日にATMでお金を下した人物の中から両方を行った者を表示。
SELECT *
FROM people
INNER JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199)
AND people.license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');
-- +--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          |
-- | 396669 | Iman  | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       | 396669    | 2014          |
-- | 467400 | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | 467400    | 2014          |
-- +--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+



-- 事件翌日に飛行機に乗った人物を調べるために空港とフライトに関する情報を表示(View information about airports and flights to find out who was on the plane the day after the incident)
 .schema airports
-- CREATE TABLE airports (
--     id INTEGER,
--     abbreviation TEXT,
--     full_name TEXT,
--     city TEXT,
--     PRIMARY KEY(id)
-- );
 .schema flights
-- CREATE TABLE flights (
--     id INTEGER,
--     origin_airport_id INTEGER,
--     destination_airport_id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     PRIMARY KEY(id),
--     FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
--     FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
-- );

-- フィフティビルの町にある空港のIDを表示(Display IDs for airports in the town of Fiftyville.)
select id from airports where city = 'Fiftyville';
-- id:8

-- 事件翌日の7月29日にフィフティビル発の飛行機の一覧（List of flights departing from Fiftyville on July 29, the day after the incide）
 select *from flights where month = 7 and day = 29 order by hour asc;
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- | 43 | 8                 | 1                      | 2023 | 7     | 29  | 9    | 30     |
-- | 23 | 8                 | 11                     | 2023 | 7     | 29  | 12   | 15     |
-- | 53 | 8                 | 9                      | 2023 | 7     | 29  | 15   | 20     |
-- | 18 | 8                 | 6                      | 2023 | 7     | 29  | 16   | 0      |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- 飛行機の搭乗者情報のpassengersテーブルを表示
 .schema passengers
-- CREATE TABLE passengers (
--     flight_id INTEGER,
--     passport_number INTEGER,
--     seat TEXT,
--     FOREIGN KEY(flight_id) REFERENCES flights(id)
-- );

-- 容疑者が乗っていた飛行機があるか検索
select * from passengers inner join flights on passengers.flight_id = flights.id
where passengers.passport_number in (5773159633, 3592750733, 7049073643, 8496433585)
and flights.month = 7 and flights.day = 29;
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 18        | 3592750733      | 4C   | 18 | 8                 | 6                      | 2023 | 7     | 29  | 16   | 0      |
-- | 36        | 5773159633      | 4A   | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- | 36        | 8496433585      | 7B   | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
-- +-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+

-- 容疑者が乗っていた可能性のある飛行機の行き先を取得
select * from airports where id in (4, 6);
-- +----+--------------+-----------------------------+---------------+
-- | id | abbreviation |          full_name          |     city      |
-- +----+--------------+-----------------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport           | New York City |
-- | 6  | BOS          | Logan International Airport | Boston        |
-- +----+--------------+-----------------------------+---------------+


-- 目撃者証言よりニューヨーク行の飛行機に乗った二人に絞って調べる。
-- 二人の個人情報を再度表示
select * from people where passport_number in (5773159633, 8496433585);
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 467400 | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+


--　事件当日該当のATMで最も多くお金を引き出した人物は関係のない人物のようだった。
-- 容疑者が2名に絞れたのでもう一度パン屋のセキュリティログを調べる。
select * from bakery_security_logs where month = 7 and day = 28 and hour <= 10 and license_plate in ('4328GD8', '94KL13X');
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 232 | 2023 | 7     | 28  | 8    | 23     | entrance | 94KL13X       |
-- | 254 | 2023 | 7     | 28  | 9    | 14     | entrance | 4328GD8       |
-- | 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- +-----+------+-------+-----+------+--------+----------+---------------+

-- 2時間店にいたほうが怪しいのでそちらを調べる。
 select * from people where license_plate = '94KL13X';
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+

-- Bruceが当日かけた電話について調べる。
select * from phone_calls where month = 7 and day = 28 and caller = '(367) 555-5533';
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2023 | 7     | 28  | 45       |
-- | 236 | (367) 555-5533 | (344) 555-9601 | 2023 | 7     | 28  | 120      |
-- | 245 | (367) 555-5533 | (022) 555-4052 | 2023 | 7     | 28  | 241      |
-- | 285 | (367) 555-5533 | (704) 555-5790 | 2023 | 7     | 28  | 75       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- 電話を掛けた相手を表示。
select * from people where phone_number in ('(375) 555-8161', '(344) 555-9601', '(022) 555-4052', '(704) 555-5790');
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 315221 | Gregory | (022) 555-4052 | 3355598951      | V4C670D       |
-- | 652398 | Carl    | (704) 555-5790 | 7771405611      | 81MZ921       |
-- | 864400 | Robin   | (375) 555-8161 | NULL            | 4V16VO0       |
-- | 985497 | Deborah | (344) 555-9601 | 8714200946      | 10I5658       |
-- +--------+---------+----------------+-----------------+---------------+

-- 電話の相手の行動を調べる。
 select * from bakery_security_logs where month = 7 and day = 28 and hour <= 10 and license_plate in ('V4C670D', '81MZ921', '4V16VO0', '10I5658');
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 248 | 2023 | 7     | 28  | 8    | 50     | entrance | 4V16VO0       |
-- | 249 | 2023 | 7     | 28  | 8    | 50     | exit     | 4V16VO0       |
-- +-----+------+-------+-----+------+--------+----------+---------------+
