# SQL reference

Written by [Saransh Halwai](https://saranshhalwai.me) with help of [Gemini](https://gemini.google.com) and [Gemini](https://antigravity.google/product/antigravity-cli). Examples Taken from [Leetcode's SQL 50](https://leetcode.com/studyplan/top-sql-50/).

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

5. [Count Salary Categories](https://leetcode.com/problems/count-salary-categories)

   ```SQL
   WITH CategorySkeleton AS (
       -- Create an on-the-fly table containing all mandatory categories
       SELECT * FROM (VALUES ('Low Salary'), ('Average Salary'), ('High Salary')) AS t(category)
   ),
   CategorizedAccounts AS (
       SELECT 
           CASE 
               WHEN income < 20000 THEN 'Low Salary'
               WHEN income BETWEEN 20000 AND 50000 THEN 'Average Salary'
               ELSE 'High Salary'
           END AS category,
           COUNT(*) AS accounts_count
       FROM Accounts
       GROUP BY 1
   )
   SELECT c.category, 
          COALESCE(a.accounts_count, 0) AS accounts_count
   FROM CategorySkeleton c
   LEFT JOIN CategorizedAccounts a USING(category);
   ```

   * Uses `VALUES` inside a CTE (`CategorySkeleton`) to build a temporary master table of all categories on-the-fly.
   * Joins the real counts table (`CategorizedAccounts`) to the skeleton using `LEFT JOIN` and `COALESCE` to guarantee that categories with zero records are still included in the final output (with a count of `0`).

6. [Group Sold Products By The Date](https://leetcode.com/problems/group-sold-products-by-the-date)

   ```SQL
   WITH UniqueActivities AS (
       -- Step 1: Evict duplicate sales of the same product on the same day
       SELECT DISTINCT sell_date, product
       FROM Activities
   )
   -- Step 2: Group by date and roll up the strings
   SELECT sell_date,
          COUNT(product) AS num_sold,
          STRING_AGG(product, ',' ORDER BY product ASC) AS products
   FROM UniqueActivities
   GROUP BY sell_date
   ORDER BY sell_date;
   ```

   * **String Aggregation**: Used to aggregate strings from multiple rows into a single string (separated by a delimiter like a comma).
     * PostgreSQL: `STRING_AGG(column, delimiter [ORDER BY clause])`
     * MySQL: `GROUP_CONCAT([DISTINCT] column [ORDER BY clause] [SEPARATOR delimiter])`

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

8. `LENGTH()` & String Manipulation (`UPPER`, `LOWER`, `LEFT`, `SUBSTRING`, `||`)

   * `LENGTH(str)`: Returns the length of a string.
   * `UPPER(str)` / `LOWER(str)`: Converts characters to uppercase/lowercase.
   * `LEFT(str, n)`: Extracts `n` characters from the left side of a string.
   * `SUBSTRING(str FROM pos)`: Extracts a substring starting from character position `pos`.
   * `||`: Concatenates two or more strings together.

   Ex 1 (Filtering by length):

   ```SQL
   SELECT t.tweet_id 
   FROM Tweets T
   WHERE LENGTH(t.content) > 15;
   ```

   Ex 2 (Capitalizing only the first character):

   ```SQL
   SELECT user_id,
          -- Force ONLY the absolute first character to Upper, and EVERYTHING else to Lower
          UPPER(LEFT(name, 1)) || LOWER(SUBSTRING(name FROM 2)) AS name
   FROM Users
   ORDER BY user_id;
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
    5. `WINDOW FUNCTIONS` are evaluated here.
    6. `SELECT` : The database picks which columns to show you.
    7. `ORDER BY` : The final output is sorted.

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

17. Window functions (`OVER`)
    In comparison to `GROUP BY`, window function do not replace the original rows by aggregates, so we still retain access to the original rows.

    Syntax:

    ```SQL
    SELECT 
        column1,
        FUNCTION() OVER (
            PARTITION BY column2
            ORDER BY column3
            ROWS/RANGE BETWEEN ...
        ) AS analytical_result
    FROM Table;
    ```

    * `PARTITION BY`: divides the table into mini tables(kinda like group by). If omitted, entire table is considered.
    * `ORDER BY`: obvious
    * `ROWS/RANGE BETWEEN`: Defines a moving subset of rows relative to the current row. If omitted but an `ORDER BY` is present, it defaults to all rows from the start of the partition up to the current row.

    Row functions:

    * `ROW_NUMBER()`: Assigns a unique, strict incrementing integer. Never allows duplicate ranks, breaking ties arbitrarily or based on secondary keys.
    * `RANK()`: Assigns identical ranks to ties. Skips ranks afterwards to account for the gap (classic Olympic medal logic: if there's a tie for Silver, there is no 3rd place).
    * `DENSE_RANK()`: Assigns identical ranks to ties, but never skips any numbers. Ranks remain completely contiguous.
    * `LAG(col, offset)`: Grabs a value from offset rows behind the current row. Phenomenal for calculating week-over-week growth or time differences between sequential events.
    * `LEAD(col, offset)`: Reaches forward to grab a value from offset rows ahead of the current row.
    * `FIRST  VALUE(col)`: Instantly extracts the value from the absolute first row of the configured frame.
    * `LAST_VALUE(col)`: Extracts the value from the absolute last row of the configured frame.

    Ex:

    ```SQL
    SELECT user_id, page_id, view_timestamp,
           LEAD(view_timestamp, 1) OVER (
               PARTITION BY user_id 
               ORDER BY view_timestamp
           ) - view_timestamp AS time_spent_on_page
    FROM UserClicks;
    

    WITH RankedDeliveries AS (
        SELECT order_date, 
               customer_pref_delivery_date,
               ROW_NUMBER() OVER(
                   PARTITION BY customer_id 
                   ORDER BY order_date, delivery_id
               ) as rn
        FROM Delivery
    )
    SELECT 
        ROUND(
            (COUNT(CASE WHEN order_date = customer_pref_delivery_date THEN 1 END) * 100.0) 
            / COUNT(*), 
            2
        ) AS immediate_percentage
    FROM RankedDeliveries
    WHERE rn = 1;
    ```

## Regex

### PostgreSQL native Operators

| Operator | Meaning | Ex | Evaluates to |
| --- | --- | --- | --- |
| `~` | Matches pattern (Case sensitive) | `'Apple' ~ 'A'` | True |
| `~*` | Matches pattern (Case insensitive) | `'Apple' ~* 'a'` | True |
| `!~` | Does not match pattern (Case Sensitive) | `'Apple' !~ 'z'` | True |
| `!~*` | Does not match pattern (Case insensitive) | `'Apple' !~* 'A'` | False |

### The characters

#### Anchors

Match positions in the string.

* `^`: Start of the string line.
* `$`: End of the string line.

#### Character classes

Can look for categories of data.

* `.`: Matches any single character(except newline)
* `\d`: Any numerical digit
* `\w`: Any alphanumeric word character(letters, numbers, or underscores)
* `\s`: Any whitespace character(spaces, tabs, newlines)

#### Quantifiers

Declare how many repetitions are allowed.

* `*`: >=0
* `+`: >0
* `?`: <=1
* `{n}`: =n

#### Sets and Groups

* `[A-Z]`: A character set. Matches any *single* uppercase letter between A and Z.
* `[^0-9]`: A negated character set. Matches any character that is not a (here) number (notice how ^ inside brackets means "NOT", but outside brackets means "START").
* `|`: OR as in C++
* `()`: Capturing group. Pins characters together so you can apply operators or quantifiers to the whole block.

### Examples

```SQL
-- Find users with valid e-mails (Leetcode 1517)
SELECT email
FROM Users
WHERE email ~ '^[A-Za-z][A-Za-z0-9_.-]*@leetcode\.com$';
```

* **Explanation**:
  * `^[A-Za-z]`: Must start with a letter (case-insensitive).
  * `[A-Za-z0-9_.-]*`: Followed by any combination of letters, digits, underscores, dots, or hyphens.
  * `@leetcode\.com$`: Must end exactly with `@leetcode.com`. The backslash `\.` is crucial because a plain `.` matches *any* character in Regex.

```SQL
-- Patients with a specific condition (Leetcode 1527)
SELECT patient_id, patient_name, conditions
FROM Patients 
WHERE conditions ~ '(^|\s)DIAB1';
```

* **Explanation**:
  * `(^|\s)`: Matches either the start of the text (`^`) OR a space (`\s`). This ensures `DIAB1` is a distinct code (e.g., matches `DIAB100` or `ACNE DIAB100` but correctly rejects `SADIAB100`).

### Key Gotchas & Dialect Differences

1. **MySQL Syntax**:
   * MySQL uses the `REGEXP` or `RLIKE` operators.
   * Example: `WHERE email REGEXP '^[A-Za-z]'`
   * MySQL's regex is **case-insensitive** by default (based on table collation). To force case sensitivity, use the `BINARY` keyword: `WHERE email REGEXP BINARY '^[A-Z]'`.

2. **Double Escaping (`\\`)**:
   * Depending on database settings or client drivers, you may sometimes need to escape backslashes in string literals (e.g. using `\\s` instead of `\s` and `\\d` instead of `\d`). This is because the SQL parser consumes one backslash before the regex engine sees it.

## References

* [PostgreSQL Docs](https://www.postgresql.org/docs/current/sql.html)
