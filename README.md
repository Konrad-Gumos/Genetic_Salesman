# Genetic Algorithm - Travelling Salesman Problem (TSP)

Project developed as part of university coursework. The goal is to solve the **Travelling Salesman Problem (TSP)** using a **genetic algorithm**.

## Project description

The program randomly generates cities on a plane and then uses a genetic algorithm to search for the shortest possible route that visits each city exactly once and returns to the starting point.

## Key assumptions

- **Number of cities:** customizable using environment variable *CITIES_N*
- **Population size:** customizable using environment variable *POPULATION* 
- **Number of generations:** runs until the result stabilizes
- **Mutation probability:** initially 5%, dynamically increased during execution
- **Output format:** results saved to `.txt` files

## Files generated

- `Punkty.txt` – coordinates of the generated cities
- `Sumy.txt` – best route distances across generations
- `Trasy.txt` – best routes across generations

## How to run the project

1. **Compilation**  
   The project requires a C++11 (or newer) compiler. You can compile it using Visual Studio or via terminal:
   ```bash
    g++ -std=c++11 main.cpp -o GeneticAlgorithm

2. **Execution**
  After compilation, run the program:
    ```bash
    ./GeneticAlgorithm
    ```

### Dependencies
No external libraries required – the project uses only the standard C++ library (<vector>, <iostream>, <cmath>, <fstream>, etc.).

### Results
Output files will be saved in the project directory.
(If needed, you can change the output paths in the functions savePointsToFile, saveSumsToFile, and saveRoutetoFile.)

## Example outcome 

For given parameters:
- **Number of cities:** customizable using environment variable *CITIES_N*
- **Population size:** customizable using environment variable *POPULATION* 
- **Number of generations:** runs until the result stabilizes
- **Mutation probability:** initially 5%, dynamically increased during execution up to 50%

![obraz](https://github.com/user-attachments/assets/2e7296a4-f111-4852-8298-d1f3ab12ecfe)
![obraz](https://github.com/user-attachments/assets/07db9b3b-d898-45e3-8a97-7572554dc9c9)
![obraz](https://github.com/user-attachments/assets/127f14e2-ea7d-42a6-8f96-7bd43f47b096)
![Genetic Algo](https://github.com/user-attachments/assets/a2619b2e-f9ce-4e04-872d-2af822178120)


## Notes

The algorithm uses a fixed random seed (srand(123)) to produce repeatable results. This can be easily disabled by de-commenting line *srand( static_cast<unsigned int>( time( 0 ) ) );* and commenting *srand( 123 );*
To visualize the routes and distance progress, a separate visualization tool (e.g., Python + matplotlib) is recommended.
