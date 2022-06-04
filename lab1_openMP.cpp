#include <omp.h>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <vector>

using namespace std;

string file_name="text1.txt";
map <string, int> words_container;
vector<string> words_array;

int main(){

        cout<<"Введите имя файла \n";
        cin>>file_name;
        int N;
        string input_word;

        cout<<"Введите число слов \n";
        cin>>N;
        cout<<"Ввод искомых слов \n";
        for(int i=0; i<N; i++){
          cin>>input_word;
          words_container.insert({{input_word,0}});
          words_array.push_back(input_word);
        }

        // замерим время выполнения
        auto start = std::chrono::system_clock::now();
        std::chrono::duration<double> diff;

        // для измерения запустим 10.тыс раз
        for(int j=0; j<10000; j++){

            #pragma omp parallel for
            for (int i=0; i<N; i++) {

                string word = words_array[i];
                ifstream readFile(file_name);
                //int num = omp_get_thread_num();
                //cout<<"i am thread "<<num<<" my word is "<<word<<"\n";
                string textLine;
                int count=0;
                int word_lenght = word.length();
                while (getline(readFile,textLine)) {
            
                    int s_pos=0;
                    int word_pos=0;

                    while (word_pos!= -1) {
                
                        word_pos=textLine.find(word, s_pos);
                        s_pos = word_pos+word_lenght;
                        if(word_pos!= -1){
                            count=count+1;
                        }

                    }            
                }
                readFile.close();
                words_container[word]=count;
            }

        }

        // конец выполнения
        auto end = std::chrono::system_clock::now();
        diff = (end - start)/10000; // рассчитаем среднее время
        std::cout << "average time: " << diff.count() << " s\n";
        
        for (const auto& n : words_container) {
            cout << n.first << " = " << n.second << "; ";
        }


    return 0;
}