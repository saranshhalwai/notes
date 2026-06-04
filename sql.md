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

3. [Students and Examinations](https://leetcode.com/problems/students-and-examinations)

   ```SQL
   WITH ExamCounts AS (
    -- Step 1: Compress the exam table down to unique pairs quickly
      SELECT student_id, subject_name, COUNT(*) AS attended_exams
      FROM Examinations
      GROUP BY student_id, subject_name
   )
   -- Step 2: Combine the small master grid with our pre-calculated counts
   SELECT 
      s.student_id, 
      s.student_name, 
      sub.subject_name, 
      COALESCE(e.attended_exams, 0) AS attended_exams
   FROM Students s
   CROSS JOIN Subjects sub
   LEFT JOIN ExamCounts e USING(student_id, subject_name)
   ORDER BY s.student_id, sub.subject_name;
   ```

   * Makes a table for student and subject pairs inside the Examinations table storing counts.
   * Uses this new table to fill in the Cartesian product table quickly.

4. [Average Selling Price](https://leetcode.com/problems/average-selling-price)

   ```SQL
   SELECT p.product_id, 
          COALESCE(
              ROUND(
                  SUM(u.units * p.price)::numeric / SUM(u.units), 
                  2
              ), 
              0
          ) AS average_price
   FROM Prices p
   LEFT JOIN UnitsSold u 
       ON p.product_id = u.product_id 
       AND u.purchase_date BETWEEN p.start_date AND p.end_date
   GROUP BY p.product_id;
   ```

   * Uses `LEFT JOIN` combined with `BETWEEN` to match sales transactions only within the active price period.
   * Uses `COALESCE` to return `0` if a product has no sales (since `SUM(u.units)` would be `NULL` and cause the average to be `NULL`).

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

14. `WITH` clause is essentially assigning a table to a variable.

    Syntax:

    ```SQL
    WITH temporary_variable_name AS (
    -- You write any query you want inside these parentheses
    SELECT column1, column2 
    FROM SomeTable
    WHERE condition
    )
    -- Now you can treat 'temporary_variable_name' like an actual table
    SELECT * FROM temporary_variable_name;
    ```

    * Written top to bottom(so goated).
    * Can be chained(lesgooo).

       ```SQL
       WITH StepOne AS (
          SELECT user_id, active_days FROM Users WHERE active_days > 10
       ),
       StepTwo AS (
       -- This CTE is filtering data that was already filtered by StepOne!
          SELECT user_id FROM StepOne WHERE user_id IN (SELECT user_id FROM PremiumUsers)
       )
       SELECT * FROM StepTwo;
       ```

    * Doesn't actually save to disk so no cleanup required.

15. `HAVING` (And how an SQL query executes)

    The logical execution order:
    1. `FROM / JOIN` : The database grabs the tables and stitches them together.
    2. `WHERE` : The database filters out individual rows (e.g., deleting deactivated employees). **Crucial note**: At this exact split second, the database has no idea how many total rows exist; it is just reading them line-by-line.
    3. `GROUP BY` : The database takes the remaining rows and sorts them into organized buckets (e.g., grouping direct reports under their specific managerID).
    4. `HAVING` : Now that the buckets are fully formed, the database can finally run math on them (like COUNT, SUM, or AVG). HAVING looks at each bucket as a single unit and filters out the entire bucket if it doesn't meet the condition.
    5. `SELECT` : The database picks which columns to show you.
    6. `ORDER BY` : The final output is sorted.

    So `HAVING` is essentially like `WHERE`, but for aggregates. It's the step where we can use functions like `COUNT`, `AVG` and others as a filter.

    Ex:

    ```SQL
    SELECT mgr.name 
    FROM Employee mgr
    JOIN Employee e ON e.managerID = mgr.id
    GROUP BY mgr.id, mgr.name
    HAVING COUNT(e.id) >= 5;
    ```

16. `BETWEEN` & Datetime Operations

    Used to check if a value (often a date/timestamp or numeric value) lies within a specific range (inclusive).

    Ex:

    ```SQL
    SELECT p.product_id, 
           COALESCE(
               ROUND(
                   SUM(u.units * p.price)::numeric / SUM(u.units), 
                   2
               ), 
               0
           ) AS average_price
    FROM Prices p
    LEFT JOIN UnitsSold u 
        ON p.product_id = u.product_id 
        AND u.purchase_date BETWEEN p.start_date AND p.end_date
    GROUP BY p.product_id;
    ```

## References

* [PostgreSQL Docs](https://www.postgresql.org/docs/current/sql.html)
