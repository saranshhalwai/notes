# SQL reference

Examples Taken from [Leetcode's SQL 50](https://leetcode.com/studyplan/top-sql-50/)

## At a glance

1. [Rising Temperature](https://leetcode.com/problems/rising-temperature)

   ```SQL
   -- PostgreSQL query
   SELECT t.id
   FROM Weather t
   JOIN Weather prev ON t.recordDate = prev.recordDate + 1
   WHERE t.temperature > prev.temperature;
   ```

   Variations of Date Arithmetic:
   * PostgreSQL: `prev.recordDate + INTERVAL '1 day'` or `prev.recordDate + 1`
   * MySQL: `DATEDIFF(t.recordDate, prev.recordDate) = 1` or `DATE_ADD(prev.recordDate, INTERVAL 1 DAY)`

   Also, a variation using the `LAG()` Window Function (avoids the self-join):

   ```SQL
   SELECT id
   FROM (
       SELECT id, 
              temperature,
              recordDate,
              LAG(temperature) OVER (ORDER BY recordDate) as prev_temp,
              LAG(recordDate) OVER (ORDER BY recordDate) as prev_date
       FROM Weather
   ) t
   WHERE temperature > prev_temp 
     AND recordDate = prev_date + 1;
   ```

2. [Average Time of process per machine](https://leetcode.com/problems/average-time-of-process-per-machine)

   ```SQL
   SELECT machine_id,
          ROUND(
              (
                  (SUM(CASE WHEN activity_type = 'end' THEN timestamp END) - 
                   SUM(CASE WHEN activity_type = 'start' THEN timestamp END)) 
                  / COUNT(DISTINCT process_id)
              )::numeric, 3
          ) AS processing_time
   FROM Activity
   GROUP BY machine_id;
   ```

   * The query aggregates all start and end time into different 'buckets' and then divides by no of distinct processes to get average directly.
   * Placing it inside `SUM()` enables **conditional aggregation** (only summing values that match the condition) which avoids doing a self-join.

## Syntax Reference

1. `SELECT` for picking rows.
2. `FROM` table. Can also alias as `FROM table t`.
3. `WHERE` for selecting rows. `AND` and `OR` can be used to group clauses.

   Ex:

   ```SQL
   SELECT product_id 
   FROM Products 
   WHERE low_fats = 'Y' AND recyclable = 'Y';
   ```

4. `COALESCE` function picks stuff in order.

   Ex:

   ```SQL
   SELECT name 
   FROM Customer 
   WHERE COALESCE(referee_id, 0) <> 2; 
   -- Here COALESCE will pick referee_id. If null, it will pick 0 instead.  
   ```

5. `UNION` runs two separate queries and merges, filtering out duplicates.

   Ex:

   ```SQL
   SELECT name, population, area FROM World WHERE area >= 3000000
   UNION
   SELECT name, population, area FROM World WHERE population >= 25000000;
   ```

6. `DISTINCT` when you need to filter out duplicates

7. `ORDER BY`

   Ex:

   ```SQL
   SELECT DISTINCT author_id AS id 
   FROM Views
   WHERE author_id = viewer_id
   ORDER BY id ASC;
   ```

8. `LENGTH()` function gives length of string.

   Ex:

   ```SQL
   SELECT t.tweet_id 
   FROM Tweets T
   WHERE LENGTH(t.content) > 15;
   ```

9. `JOIN`

   Ex:

   ```SQL
   SELECT u.unique_id, e.name
   FROM Employees e
   LEFT JOIN EmployeeUNI u ON e.id = u.id;
   ```

   Also, a variation with `USING` when both tables share a column(perf wise same):

   ```SQL
   SELECT u.unique_id, e.name
   FROM Employees e
   LEFT JOIN EmployeeUNI u USING(id);
   ```

10. `GROUP BY` to aggregate by some column. All columns should be grouped by **must** be included.
  
    Ex:
  
    ```SQL
    SELECT v.customer_id, COUNT(v.visit_id) AS count_no_trans
    FROM Visits v
    LEFT JOIN Transactions t USING(visit_id)
    WHERE t.transaction_id IS NULL
    GROUP BY v.customer_id;
    ```

11. `ROUND`
    Ex:

    ```SQL
    SELECT a.machine_id, 
          ROUND(AVG(b.timestamp - a.timestamp)::numeric, 3) AS processing_time
    FROM Activity a
    JOIN Activity b USING(machine_id, process_id)
    WHERE a.activity_type = 'start' AND b.activity_type = 'end'
    GROUP BY a.machine_id;
    ```

12. `CASE WHEN` (Conditional Logic & Conditional Aggregation)

    Similar to if-else or switch-case in programming languages. Two types
    1. Searched `CASE` (like if-else-if)

       Ex:

       ```SQL
       SELECT customer_id,
       CASE 
           WHEN total_spent >= 10000 THEN 'VIP Platinum'
           WHEN total_spent >= 5000  THEN 'Gold'
           WHEN total_spent >= 1000  THEN 'Silver'
           ELSE 'Bronze'
       END AS loyalty_tier
       FROM CustomerSpending;
       ```

    2. Simple `CASE`

       ```SQL
       SELECT order_id,
       CASE status_code
           WHEN 1 THEN 'Pending'
           WHEN 2 THEN 'Shipped'
           WHEN 3 THEN 'Delivered'
           ELSE 'Cancelled'
       END AS order_status
       FROM Orders;
       ```

    Rules:
    * Only the first condition that matches is evaluated. Rest is ignored.
    * If no explicit `ELSE` statement then returns `NULL` by default.
    * Types returned types must be consistent else error.
13. `CROSS JOIN` to get all possible combinations.
    Ex:

    ```SQL
    SELECT s.student_id, s.student_name, sub.subject_name, COUNT(e.subject_name) AS attended_exams
    FROM Students s
    CROSS JOIN Subjects sub
    LEFT JOIN Examinations e ON s.student_id = e.student_id AND sub.subject_name = e.subject_name
    GROUP BY s.student_id, s.student_name, sub.subject_name
    ORDER BY s.student_id, sub.subject_name;
    ```

## References

* [PostgreSQL Docs](https://www.postgresql.org/docs/current/sql.html)
