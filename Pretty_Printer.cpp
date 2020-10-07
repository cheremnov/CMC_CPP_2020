#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
/**
 * A pretty printer
 */
struct Printer {
	// возвращает строковое представление
	std::string str() const{
		return printer_value_.str();
	}
	/**
	 * Pretty print iterable containers
	 */
	template <typename T>
	void formatIterable( const T& iterable){
		// Don't append the "," for the first element
		bool is_first_elem = true;
		for( auto& elem: iterable ){
			if( is_first_elem ){
				is_first_elem = false;
			} else{
				printer_value_ << ", ";
			}
			printer_value_ << Printer().format( elem).str();
		}
	}
	// далее перегруженные/шаблонные функции вида:
	template <typename T1, typename T2>
	Printer& format( const std::pair<T1, T2>& t){
		printer_value_ << "(" << Printer().format( t.first).str() << ", " << 
			Printer().format( t.second).str() << ")";
		return *this;
	}
	template <typename... Args>
	Printer& format( const std::tuple<Args...>& tuple_val){
		printer_value_ << "(";
		std::apply([this](auto&&... args) {
			// Fold expression
			(( printer_value_ << Printer().format( args).str() << ", "),...); 
		}, tuple_val);
		/** 
		 * Delete the last "," in the stream
		 * Actually seekp doesn't delete anything,
		 * but the ")" overwrites ","
		 */
		printer_value_.seekp( -2, printer_value_.cur);
		printer_value_ << ") ";
		return *this;
	}
	/** 
	 * Pretty print a vector
	 */
	template <typename T>
	Printer& format( const std::vector<T>& vec){
		printer_value_ << "[";
		formatIterable<std::vector<T> > ( vec);
		return *this;
	}
	/** 
	 * Pretty print a set
	 */
	template <typename T>
	Printer& format( const std::set<T>& set_val){
		printer_value_ << "{";
		formatIterable<std::set<T> >( set_val);
		printer_value_ << "}";
		return *this;
	}
	/**
	 * The default pretty printer for the printable types
	 */
	template <typename T> 
	Printer& format( const T& printable){
		printer_value_ << printable;
		return *this;
	}
private:
	std::stringstream printer_value_;
};
template<typename T>
std::string format(const T& t) {
	return Printer().format(t).str();
}
int main() {
std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
std::tuple<std::string, int, int> t = {"xyz", 1, 2};
std::cout << Printer().format("start ").format(std::make_pair(2, "ab")).format(" stop").str() << std::endl;
std::set< std::tuple< int, double> > s= { std::make_tuple( 1, 2.5), std::make_tuple( 2, 3.5) };
std::cout << Printer().format(t).format(" ! ").format(0).str() << std::endl;
std::cout << Printer().format(" vector: ").format(v).str() << std::endl;
std::cout << Printer().format(" set: ").format(s).str() << std::endl;
}