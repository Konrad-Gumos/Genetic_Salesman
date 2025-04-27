#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <fstream>

#define CITIES_N 100
#define POPULATION 500
//#define MUT_PROP 15
#define GENERATIONS 500

using namespace std;

int MUT_PROP = 5;

struct SumList
{
  vector<int> route;
  double sumDist;

  SumList( const vector<int>& route, double sumDist ): route( route ), sumDist( sumDist ) {}

};

void savePointsToFile( const vector<pair<int, int>>& points)
{
  string filename = "Punkty.txt";
  ofstream file( filename );

  for( const auto& p : points )
  {
    file << p.first << "," << p.second << "\n";
  }

  file.close();
  cout << "Zapisano " << points.size() << " punktów do pliku " << filename << endl;
}

void saveSumsToFile( const vector<int>& sums )
{
  string filename = "Sumy.txt";
  ofstream file( filename );

  for( const auto& p : sums )
  {
    file << p << "\n";
  }

  file.close();
  cout << "Zapisano " << sums.size() << " sum do pliku " << filename << endl;
}

void saveRoutetoFile(const vector<vector<int>>& routes)
{
  string filename = "Trasy.txt";
  ofstream file( filename );

  for( const auto& p : routes )
  {
    file << p[0];
    for( int i = 1; i < p.size(); i++ )
    {
      file << "," << p[i] ;
    }
    file << "\n";
  }

  file.close();
  cout << "Zapisano " << routes.size() << " sciezek do pliku " << filename << endl;
}

void randCities(vector<pair<int,int>>& points) {
  // Drawing location of cities
  //srand( static_cast<unsigned int>( time( 0 ) ) );
  srand( 123 );

  // Randomly generate cities
  for( int i = 0; i < CITIES_N; i++ )
  {
    int x = rand() % 100 + 1;
    int y = rand() % 100 + 1;
    
    //circle
    //int y = sqrt( pow( 50, 2 ) - pow( x-50, 2 ) );
    //if( rand() % 2 == 0 ) y = -y;
    //y += 50;

    points.push_back( make_pair( x, y ) );
    //printf( "Wylosowane zmienne: x: %d,y: %d\n", x, y );
  }
  savePointsToFile( points );
}

//create a matrix of distances between cities
void createDistMatrix(const vector<pair<int, int>>& points, vector<vector<double>>& matrix) {
  int n = static_cast<int>(points.size());
  matrix.resize( n, vector<double>( n ) );
  for( int i = 0; i < n; i++ )
  {
    for( int j = 0; j < n; j++ )
    {
      matrix[i][j] = sqrt( pow( ( points[i].first - points[j].first ), 2 ) + pow( ( points[i].second - points[j].second ), 2 ) );
    }
  }
}

template<typename T>
void printMatrix(vector<vector<T>>& matrix)
{
  int X = matrix.size();
  int Y = matrix[0].size();

  for( int i = 0; i < X; i++ )
  {
    for( int j = 0; j < Y; j++ )
    {
      cout << "\t" << matrix[i][j];
    }
    cout << endl;
  }
}

void calcDistSum( vector<vector<int>> pop, vector<vector<double>> dist, vector<SumList>& list )
{
  for( int i = 0; i < pop.size(); i++ ) //iterate through population of routes
  {
    double sum = 0; //variable used to store complete distance on route
    for( int k = 0; k < pop[i].size(); k++ ) //iterate through cities of one route
    {
      sum += dist[pop[i][k] - 1][pop[i][(k+1)%pop[i].size()] - 1]; //sum the distance between cities
    }
    //std::cout << sum << endl;
    list.push_back( SumList(pop[i], sum) );
  }
}

vector<SumList> selectParents( vector<SumList>& list )
{
  //sort by distance
  sort( list.begin(), list.end(), []( const SumList& a, const SumList& b )
  {
    return a.sumDist < b.sumDist;
  } );

  vector<SumList> selected( list.begin(), list.begin() + list.size() / 2);
  return selected;
}

bool isElement( int elem, vector<int> list )
{
  for( int i = 0; i < list.size(); i++ )
  {
    if( elem == list[i] ) return true;
  }
  return false;
}

vector<vector<int>> createChildren( vector<SumList> list )
{
  vector<vector<int>> children;
 
  int route_n = static_cast<int>(list[0].route.size());
  children.resize( POPULATION, vector<int>( route_n ) );

  //crossing
  for( int i = 0; i < list.size()-1; i += 2 )
  {
    int begin = rand() % route_n;
    int end = rand() % route_n;
    int mut1 = rand() % 100 + 1;
    int mut2 = rand() % 100 + 1;

    //switch in case begin>end
    if( begin > end )
    {
      int temp = begin;
      begin = end;
      end = temp;
    }
    //check if index in bounds
    if( begin >= route_n || end >= route_n ) std::cout << "Index out of bounds for crossing gens" << endl;

    //========CROSS========//
    // 
    //getting gene from parent#1 and parent#2
    vector<int> gens;
    vector<int> gens2;
    for( int j = begin; j <= end; j++ )
    {
      gens.push_back( list[i].route[j] );
      gens2.push_back( list[i+1].route[j] );
    }

    //completing genes for children
    int k = 0;
    int k2 = 0;
    int k3 = 0;
    while( k < route_n )
    {
      if( k >= begin && k <= end )
      {
        children[i][k++] = gens[k3++];
        continue;
      }

      while( isElement( list[i + 1].route[k2], gens ) ) k2++;
      children[i][k] = list[i + 1].route[k2];

      k++;
      k2++;
    }

    k = 0;
    k2 = 0;
    k3 = 0;
    while( k < route_n )
    {
      if( k >= begin && k <= end )
      {
        children[i+1][k++] = gens[k3++];
        continue;
      }

      while( isElement( list[i].route[k2], gens ) ) k2++;
      children[i+1][k] = list[i].route[k2];

      k++;
      k2++;
    }
    //========================//
    //=======MUTATE===========//
    //
    if( mut1 <= MUT_PROP )
    {
      int gene_sw = rand() % route_n;
      int gene_sw2 = rand() % route_n;

      int temp = children[i][gene_sw];
      children[i][gene_sw] = children[i][gene_sw2];
      children[i][gene_sw2] = temp;
    }
    if( mut2 <= MUT_PROP )
    {
      int gene_sw = rand() % route_n;
      int gene_sw2 = rand() % route_n;

      int temp = children[i+1][gene_sw];
      children[i+1][gene_sw] = children[i+1][gene_sw2];
      children[i+1][gene_sw2] = temp;
    }
  }

  //completing population with best units from previous one
  for( int i = static_cast<int>(list.size()); i < POPULATION; i++ )
  {
    children[i] = list[i - (list.size())].route;
  }

  return children;
}

void mutInc()
{
  if( MUT_PROP < 100 )
  {
    MUT_PROP += 5;
  }
}

int main()
{
  vector<pair<int, int>> points;
  randCities(points);
  vector<vector<double>> dist;
  createDistMatrix( points, dist ); 

  //entry sequence of numbers
  vector<int> seq( CITIES_N );
  int startNum = 0;
  int step = 1;
  generate( seq.begin(), seq.end(), [&startNum, &step] { return startNum += step; } );

  //choosing random routes between cities and saving it as pop elements
  vector<vector<int>> pop;
  for( int i = 0; i < POPULATION; i++ )
  {
    random_shuffle(seq.begin(), seq.end());
    pop.push_back(seq);
  }

  vector<int> bestSums;
  vector<vector<int>> bestRoutes;
  //for( int generations = 0; generations < GENERATIONS+1; generations++ )
  while(true)
  {
    vector<SumList> sumList;
    calcDistSum( pop, dist, sumList );

    //new gens
    vector<SumList> parents;
    parents = selectParents( sumList );
    pop = createChildren( parents );

    //save data to files
    bestSums.push_back( parents[0].sumDist );
    bestRoutes.push_back( parents[0].route );

    if( bestSums.size() == 1 ) cout << "Suma dystansow: " << bestSums[0] << endl;
    
    //end condition
    if( bestSums.size() >= 100 )
    {
      if( bestSums[bestSums.size() - 1] - bestSums[bestSums.size() - 50] < 5 ) mutInc();

      if( bestSums[bestSums.size() - 1] == bestSums[bestSums.size() - 100] )
      {
        cout << "Pokolenie: " << bestSums.size()-1 << ". Suma dystansow: " << bestSums[bestSums.size() - 1] << endl;
        break;
      }
    }
  }

  saveSumsToFile( bestSums );
  saveRoutetoFile( bestRoutes );

  return 0;
}