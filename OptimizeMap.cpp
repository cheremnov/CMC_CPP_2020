/**
 * Use hint to accelerate std::map
 */
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <map>
#include <chrono>

typedef std::chrono::milliseconds ms;
typedef std::chrono::high_resolution_clock chrono_clock;

/**
 * What type of hint use in the std::map
 */
typedef enum {
    HINT_NONE,
    // Hint to the end of a key set
    HINT_END,
    // Hint to the last inserted key
    HINT_LAST_INSERTED
} HintType_t;
/**
 * Fill a map with N random integers,
 * from the interval [start; end)
 */
void fillMap( std::map<int, int>& example_map, 
              int samples_number,        // Number of samples
              int start,                 // Start of the interval
              int end){                  // End of the interval 
    assert( end > start);
    for( int times = 0; times < samples_number; ++times ){
        int random_number = start + rand() % (end - start);
        example_map[random_number] = 1;
    }
}
/**
 * Add to a map keys with values from the interval [start; end)
 */
void fillMapInterval( std::map<int, int>& example_map, int start, int end, 
    HintType_t hint_type = HINT_NONE){
    assert( end > start);
    std::map<int, int>::iterator last_inserted = example_map.begin();
    for( int key = start; key < end; ++key ){
        if( hint_type == HINT_NONE ){
            example_map.insert( std::pair<int,int> (key, 1));
        } else if( hint_type == HINT_END ){
            example_map.insert( example_map.end(), std::pair<int,int> (key, 1));
        } else if( hint_type == HINT_LAST_INSERTED){
            last_inserted = example_map.insert( last_inserted, std::pair<int,int> (key, 1));
        }
    }
}
enum { RANDOM_SEED = 42 };
int main(){
    std::srand( RANDOM_SEED);
    std::chrono::time_point<chrono_clock> measure_begin;
    std::chrono::time_point<chrono_clock> measure_end;
    std::map<int, int> integer_map;
    enum {
        SAMPLES_NUMBER = 1000000, 
        /**
         * In the first calculation, initially all keys are less than the upper limit
         */
        UPPER_LIMIT = 10000000, 
        /**
         * In the second calculation, initially all keys are more than the lower limit
         */
        LOWER_LIMIT = 10000000, 
    };

    // Initialize a map with random values
    fillMap(integer_map, SAMPLES_NUMBER, 0, UPPER_LIMIT); 
    std::cout << "First measurement: " << std::endl;

    /**
     * Add to the integer map sorted keys,
     * that are bigger then all previous keys
     */
    measure_begin = chrono_clock::now();
    fillMapInterval( integer_map, UPPER_LIMIT, 2 * UPPER_LIMIT);
    measure_end = chrono_clock::now();
    std::cout << "No hint: " << std::chrono::duration_cast<ms>(measure_end - measure_begin).count() 
        << " milliseconds" << std::endl;
    integer_map.clear();

    /**
     * Use hint to add sorted keys to the map
     */
    fillMap(integer_map, SAMPLES_NUMBER, 0, UPPER_LIMIT); 
    measure_begin = chrono_clock::now();
    fillMapInterval( integer_map, UPPER_LIMIT, 2 * UPPER_LIMIT, HINT_END);
    measure_end = chrono_clock::now();
    std::cout << "With hint: " << std::chrono::duration_cast<ms>(measure_end - measure_begin).count()
        << " milliseconds" << std::endl;
    
    integer_map.clear();
    std::cout << "Second measurement: " << std::endl;
    fillMap(integer_map, SAMPLES_NUMBER, LOWER_LIMIT, 2 * LOWER_LIMIT); 
    /**
     * Add to the integer map sorted keys,
     * that are smaller than all previous keys
     */
    measure_begin = chrono_clock::now();
    fillMapInterval( integer_map, 0, LOWER_LIMIT);
    measure_end = chrono_clock::now();
    std::cout << "No hint: " << std::chrono::duration_cast<ms>(measure_end - measure_begin).count() 
        << " milliseconds" << std::endl;
    integer_map.clear();

    /**
     * Use a hint to add sorted keys to the map
     * Insert a new key directly after the last inserted key
     */
    fillMap(integer_map, SAMPLES_NUMBER, LOWER_LIMIT, 2 * LOWER_LIMIT); 
    measure_begin = chrono_clock::now();
    fillMapInterval( integer_map, 0, LOWER_LIMIT, HINT_LAST_INSERTED);
    measure_end = chrono_clock::now();
    std::cout << "With hint: " << std::chrono::duration_cast<ms>(measure_end - measure_begin).count()
        << " milliseconds" << std::endl;
    return 0;
}
