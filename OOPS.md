# OOPS quick reference

This is NOT a comprehensive guide. It's just the stuff you may forget.

1. **Four pillars of OOPS:**
   * **Abstraction:** Hiding complex implementation details and showing only the essential features.
   * **Encapsulation:** Binding data and methods together and restricting direct access (using access specifiers).
   * **Inheritance:** Enabling a class to acquire properties and behaviors of another class.
   * **Polymorphism:** The ability of a function/message to behave in more than one form.

2. **Overriding** requires the `virtual` keyword in the base class in C++.

3. **Overloading** is compile-time polymorphism, whereas **overriding** is run-time polymorphism.

4. A `virtual` function essentially tells the compiler to look for an implementation in the derived class.

   ```cpp
   Base* b = new Derived();
   b->show();
   ```

   Here it correctly calls `show()` of the derived class.

   * **Under the Hood (Vtable & Vptr):**
     * **Vtable (Virtual Table):** A static lookup table created by the compiler for each class containing virtual functions. It holds pointers to the virtual functions.
     * **Vptr (Virtual Pointer):** A hidden pointer added to every object instance of a class with virtual functions. It points to the class's Vtable.
     * At runtime, `b->show()` dereferences the object's `vptr` to find the correct function address in the Vtable (dynamic/late binding).

5. **Pure virtual function:**

   ```cpp
   virtual void show() = 0;
   ```
  
   Makes the class abstract (cannot be instantiated directly).

6. **Interface:** In C++, an interface is simulated as a class with *only pure virtual functions* and a virtual destructor.

7. **Virtual Destructor:** Deleting a derived object via a base pointer:
  
   ```cpp
   Base* b = new Derived();
   delete b;
   ```
  
   requires the base class destructor to be virtual to prevent memory leaks:
  
   ```cpp
   virtual ~Base() {}
   ```

8. **Diamond problem fix (Virtual Inheritance):**
  
   ```cpp
   class B : virtual public A {};
   class C : virtual public A {};
   ```

9. **Object slicing:**
  
   ```cpp
   Derived d;
   Base b = d;
   ```
  
   The derived part gets discarded. Use references or pointers to preserve polymorphic behavior:
  
   ```cpp
   Base& b_ref = d;
   ```

10. **Compile-time vs run-time polymorphism:**
    * **Compile-time:** Overloading, Templates (Early binding)
    * **Run-time:** Virtual Functions, Overriding (Late binding)

11. **`final` and `override` specifiers:**
    * `override`: Tells the compiler that the function is intended to override a virtual function in a base class (catches signature mismatches).
    * `final`:
      * On a **class**: Prevents the class from being inherited.
      * On a **virtual function**: Prevents further overriding in derived classes.
    * *(Note: Unlike Java, `final` cannot be applied to variables in C++. Use `const` or `constexpr` instead.)*
