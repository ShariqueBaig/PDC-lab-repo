# Parallel Skeletons Explained: The "Real World" Edition

If the technical definitions felt dry, let's use real-world analogies. Parallel Skeletons are just fancy names for **how we organize teamwork**.

Imagine you are a manager, and you have **4 workers** (cores) to get a job done.

---

## 1. Map Skeleton (The "Parallel Task" Team)
**Scenario**: You have a stack of 100 exams to grade.
- **The "Map" approach**: You give 25 exams to Worker A, 25 to Worker B, 25 to C, and 25 to D.
- **Key Point**: They don't need to talk to each other. Worker A doesn't care if Worker B is slow or fast. They work completely **independently**.
- **When to use**: When you have a list of things and you want to do the *same thing* to all of them (e.g., squaring numbers, converting images to black & white).

## 2. Reduce Skeleton (The "Tournament" Bracket)
**Scenario**: You want to find the **sum** of all points scored by 100 students.
- **The "Reduce" approach**:
    1.  **Round 1**: Everyone grabs a pair of test scores and adds them up. (100 scores become 50 sums).
    2.  **Round 2**: They take those 50 sums and add them in pairs. (50 sums become 25 sums).
    3.  ...and so on, until only **one** number remains.
- **Key Point**: It looks like a tennis tournament bracket. 8 players $\to$ 4 winners $\to$ 2 finalists $\to$ 1 champion.
- **When to use**: When you need to crush a big list into a **single answer** (Sum, Max, Min).

## 3. Farm Skeleton (The "Call Center")
**Scenario**: You run a customer support center. Calls (tasks) are coming in randomly.
- **The "Farm" approach**:
    - You represent the **Master** (The Queue).
    - Your workers are the **Agents**.
    - An agent finishes a call. They raise their hand. You give them the *next* waiting call.
- **Key Point**: Some calls take 1 minute, some take 20. The "Farm" ensures that if Worker A gets a hard task, Worker B can handle 3 easy tasks in the meantime. Everyone stays busy.
- **When to use**: When your tasks have **different sizes** or arrive unpredictably (e.g., processing web requests).

## 4. Pipe Skeleton (The "Car Factory" Assembly Line)
**Scenario**: Building a car.
- **The "Pipe" approach**:
    - **Worker A** puts on the chassis.
    - **Worker B** drops in the engine.
    - **Worker C** paints the car.
- **Key Point**: All three workers are working *at the same time* but on *different cars*. While C is painting Car #1, B is putting an engine in Car #2, and A is starting Car #3.
- **When to use**: When your process has distinct **stages** that data must flow through.

## 5. Divide and Conquer (The "Cleaning Crew")
**Scenario**: Cleaning a massive messy warehouse.
- **The "D&C" approach**:
    - The warehouse is too big for one person.
    - You split the room in half. "Team A, take the left side. Team B, take the right."
    - Team A finds their side is still too big, so they split it again.
    - Eventually, the area is small enough for one person to sweep quickly. Then everyone comes back together to say "Done!"
- **When to use**: For sorting algorithms (Merge Sort, Quick Sort) or big recursive problems.

## 6. Wavefront (The "Stadium Wave")
**Scenario**: Fans doing "The Wave" in a stadium, but diagonally.
- **The "Wavefront" approach**:
    - You are in seat (Row 5, Seat 5). You *cannot* stand up until the person in (Row 5, Seat 4) AND the person in (Row 4, Seat 5) have stood up.
    - The wave starts at the corner and moves diagonally across the crowd.
- **Key Point**: You have dependencies. You can't start until your neighbors are done. But everyone on the same diagonal line can stand up at the same time!
- **When to use**: Grid problems like sequence alignment (DNA matching) or physical simulations (heat moving through metal).
