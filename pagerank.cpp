#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;


vector<double> PageRank(const unordered_map<int, vector<pair<int, double>>> &graph, int numSongs, int numIterations, double dampingFactor)
{
    //Initializing the ranks and nrewRanks arrays 
    vector<double> ranks(numSongs, 1.0 / numSongs); 
    vector<double> newRanks(numSongs, 0.0);        

    
    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        //Reset newRanks for the current iteration
        for (int i = 0; i < numSongs; ++i)
        {
            newRanks[i] = (1.0 - dampingFactor) / numSongs;
        }

        //We distribute the rank of each song to all of its neighbors
        for (const auto &songPair : graph)
        {
            int song = songPair.first;
            const vector<pair<int, double>> &neighbors = songPair.second;


            double totalWeight = 0.0;
            for (const auto &neighbor : neighbors)
            {
                totalWeight += neighbor.second;
            }

            if (totalWeight > 0)
            {
                for (const auto &neighbor : neighbors)
                {
                    int neighborSong = neighbor.first;
                    double weight = neighbor.second;
                    newRanks[neighborSong] += dampingFactor * ranks[song] * (weight / totalWeight);
                }
            }
        }

        ranks = newRanks;
    }

    //Return final ranks
    return ranks;
}


void runMusicRecommendation(const unordered_map<int, vector<pair<int, double>>>& graph, int numSongs, int numIterations, double dampingFactor, const vector<string>& songTitles) {
    cout << "Running PageRank for Music Recommendation\n";

    // Calculate PageRank
    vector<double> ranks = PageRank(graph, numSongs, numIterations, dampingFactor);

    // Pair songs with their ranks
    vector<pair<string, double>> rankedSongs;
    for (int i = 0; i < ranks.size(); ++i) {
        rankedSongs.emplace_back(songTitles[i], ranks[i]);
    }

    // Sort songs by rank in descending order
    sort(rankedSongs.begin(), rankedSongs.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return b.second < a.second; // Sort by rank (higher rank first)
    });

    // Print the results
    cout << "Recommended Songs in Ranked Order:\n";
    for (const auto& song : rankedSongs) {
        cout << song.first << ": " << fixed << setprecision(4) << song.second << "\n";
    }
    cout << "-----------------------------------------\n";
}



//I added all my test cases below 
int main()
{

    vector<string> songTitles = {"Song A", "Song B", "Song C", "Song D"};

    // Test Case 1:
    unordered_map<int, vector<pair<int, double>>> graph1 = {
        {0, {{1, 1.0}, {2, 2.0}}}, // Song A -> Song B (1.0), Song A -> Song C (2.0)
        {1, {{0, 1.0}, {3, 1.0}}}, // Song B -> Song A (1.0), Song B -> Song D (1.0)
        {2, {{0, 2.0}, {3, 1.0}}}, // Song C -> Song A (2.0), Song C -> Song D (1.0)
        {3, {{1, 1.0}, {2, 1.0}}}  // Song D -> Song B (1.0), Song D -> Song C (1.0)
    };
    runMusicRecommendation(graph1, 4, 10, 0.85, songTitles);

    // Test Case 2: 
    unordered_map<int, vector<pair<int, double>>> graph2 = {
        {0, {{1, 1.0}}}, // Song A -> Song B
        {1, {{2, 1.0}}}, // Song B -> Song C
        {2, {{3, 1.0}}}, // Song C -> Song D
        {3, {{0, 1.0}}}  // Song D -> Song A
    };
    runMusicRecommendation(graph2, 4, 10, 0.85, songTitles);

    // Test Case 3:
    unordered_map<int, vector<pair<int, double>>> graph3 = {
        {0, {{1, 1.0}, {2, 2.0}}}, // Song A -> Song B (1.0), Song A -> Song C (2.0)
        {1, {{0, 1.0}}},           // Song B -> Song A (1.0)
        {2, {{0, 1.0}}},           // Song C -> Song A (1.0)
        {3, {}}                    // Song D
    };
    runMusicRecommendation(graph3, 4, 10, 0.85, songTitles);

    // Test Case 4:
    unordered_map<int, vector<pair<int, double>>> graph4 = {
        {0, {{1, 1.0}, {2, 2.0}}}, // Song A -> Song B (1.0), Song A -> Song C (2.0)
        {1, {{0, 1.0}, {3, 1.0}}}, // Song B -> Song A (1.0), Song B -> Song D (1.0)
        {2, {{0, 2.0}, {3, 1.0}}}, // Song C -> Song A (2.0), Song C -> Song D (1.0)
        {3, {{1, 1.0}, {2, 1.0}}}, // Song D -> Song B (1.0), Song D -> Song C (1.0)
        {4, {}}                    // Song E (Isolated)
    };
    vector<string> songTitles4 = {"Song A", "Song B", "Song C", "Song D", "Song E"};
    runMusicRecommendation(graph4, 5, 10, 0.85, songTitles4);

    // Test Case 5:
    unordered_map<int, vector<pair<int, double>>> graph5 = {
        {0, {{1, 1.0}, {2, 1.0}, {3, 1.0}}}, // Song A -> All other songs equally
        {1, {{0, 1.0}, {2, 1.0}, {3, 1.0}}}, // Song B -> All other songs equally
        {2, {{0, 1.0}, {1, 1.0}, {3, 1.0}}}, // Song C -> All other songs equally
        {3, {{0, 1.0}, {1, 1.0}, {2, 1.0}}}  // Song D -> All other songs equally
    };
    vector<string> songTitles5 = {"Song A", "Song B", "Song C", "Song D"};
    runMusicRecommendation(graph5, 4, 10, 0.85, songTitles5);

    // Test Case 6:
    unordered_map<int, vector<pair<int, double>>> graph6 = {
        {0, {{1, 5.0}, {2, 1.0}}}, // Song A -> Song B (5.0), Song A -> Song C (1.0)
        {1, {{0, 1.0}, {3, 2.0}}}, // Song B -> Song A (1.0), Song B -> Song D (2.0)
        {2, {{3, 3.0}}},           // Song C -> Song D (3.0)
        {3, {{0, 4.0}}}            // Song D -> Song A (4.0)
    };
    vector<string> songTitles6 = {"Song A", "Song B", "Song C", "Song D"};
    runMusicRecommendation(graph6, 4, 10, 0.85, songTitles6);




    return 0;
}
