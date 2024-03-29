-- person_idをもとに人の名前を検索する
select name from people where id in (
    -- みつけたIDに出演した人のIDを見つける
    select person_id from stars where movie_id in  (
        -- 2004年に公開された映画のIDを表示
        select id from movies where year = 2004
    )
) order by birth;
