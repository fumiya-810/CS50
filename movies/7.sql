select movies.title, ratings.rating
 from ratings
 join movies on ratings.movie_id = movies.id
 where year = 2010 order by ratings.rating desc,
 movies.title asc;
