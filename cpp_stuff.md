# C++ stuff

## Smart pointers

1. `unique_ptr`
   * Exclusive ownership. No two unique_ptrs can point to the same resource.
   * Zero memory or performance overhead compared to a raw pointer.
   * Move-only (cannot be copied). Use this by default 90% of the time.

   ```cpp
   #include <iostream>
   #include <memory>
   using namespace std;

   class Rectangle {
       int length, breadth;

   public:
       Rectangle(int l, int b) : length(l), breadth(b) {}
       int area() { return length * breadth; }
   };

   int main() {
       unique_ptr<Rectangle> P1 = make_unique<Rectangle>(10, 5);
       cout << P1->area() << endl;

       unique_ptr<Rectangle> P2;
       P2 = move(P1);  // ownership transfer

       cout << P2->area();
       return 0;
   }
   ```

2. `shared_ptr`
   * Shared ownership using reference counting via an external control block.
   * Size is twice that of a raw pointer (one pointer to object, one to the control block).
   * Use case: Genuinely unpredictable object lifetimes, like async thread pools, complex graphs, or Python/C++ bindings.

   ```cpp
   #include <iostream>
   #include <memory>
   using namespace std;

   class Rectangle {
      int length, breadth;

   public:
       Rectangle(int l, int b) : length(l), breadth(b) {}
       int area() { return length * breadth; }
   };

   int main() {
       shared_ptr<Rectangle> P1(new Rectangle(10, 5));
       shared_ptr<Rectangle> P2 = P1;

       cout << P1->area() << endl;
       cout << P2->area() << endl;
       cout << P1.use_count();
       return 0;
   }
   ```

3. `weak_ptr`
   * Non-owning observer. Points to a resource managed by a shared_ptr without bumping the reference count.
   * Used primarily to break cyclic dependencies.

   ```cpp
   #include <iostream>
   #include <memory>
   using namespace std;

   class Rectangle {
       int length, breadth;

   public:
       Rectangle(int l, int b) : length(l), breadth(b) {}
       int area() { return length * breadth; }
   };

   int main() {
       shared_ptr<Rectangle> P1(new Rectangle(10, 5));
       weak_ptr<Rectangle> P2(P1);

       cout << P1->area() << endl;
       cout << P2.use_count();
       return 0;
   }
   ```

### Note

* `make_unique` / `make_shared` vs new
  * Exception safety: Prevents memory leaks if an exception is thrown during evaluation of function arguments before assignment.\
  * Performance: std::make_shared allocates the managed object and the control block in a single contiguous memory block, reducing allocation overhead and improving cache locality.

* Cyclic Dependencies
  * If Object A owns Object B via `shared_ptr`, and Object B owns Object A via `shared_ptr`, their reference counts never reach zero. This causes a permanent memory leak. Fix this by changing one side of the relationship to a `weak_ptr`.

* Thread Safety
  * The control block (reference counter) is thread-safe and updates atomically. However, the underlying managed object itself is NOT thread-safe. Concurrent modifications to the data still require external synchronization like a mutex.

* Industry Reality
  * Default to `unique_ptr` for performance and clear ownership design. Avoid "shared pointer disease" (overusing `shared_ptr` everywhere). Game engine hot loops and competitive programming completely avoid `shared_ptr` due to atomic counting overhead and cache misses.
