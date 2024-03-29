select name from people where id in (
    select person_id from stars where movie_id in (
        select movie_id from stars where person_id in (
            select id from people where birth = 1958 and name = 'Kevin Bacon'
        )
    )
) and name != 'Kevin Bacon';
