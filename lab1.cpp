#include <iostream>
#include <pthread.h>
#include <fstream>
#include <map>
#include <chrono>
#include <sys/time.h>
#include <ctime>

using namespace std;

int main (){

    try
    {
        string file_name;
        cout<<"Введите имя файла \n";
        cin>>file_name;

        string textLine;
        map <string, int> words_container;
        int N;
        string searched_word;

        cout<<"Введите число слов \n";
        cin>>N;
        cout<<"Ввод искомых слов \n";
        for(int i=0; i<N; i++){
          cin>>searched_word;
          words_container.insert({{searched_word,0}});
        }

        bool done = false;
        std::chrono::duration<double> diff;
        // замерим время выполнения
        auto start = std::chrono::system_clock::now();

        // для измерения запустим 1 мил. раз
        for(int i=0; i<10000; i++){

            if(i==1){
                done=true;
            }

            ifstream readFile(file_name);
            while (getline(readFile,textLine))
            {
            
                for (const auto& n : words_container) {

                    int word_lenght= n.first.length();
                    int s_pos=0;
                    int word_pos=0;

                    while (word_pos!= -1)
                    {
                        word_pos=textLine.find(n.first, s_pos);
                        s_pos = word_pos+word_lenght;
                        if(word_pos!= -1 && done==false){
                            words_container[n.first]=words_container[n.first]+1;
                        }
                    }

                }
                
            }
            readFile.close();
        }

        // конец выполнения
        auto end = std::chrono::system_clock::now();
        diff = (end - start)/10000;// рассчитаем среднее время
        std::cout << "average time: " << diff.count() << " s\n";
        
        for (const auto& n : words_container) {
            cout << n.first << " = " << n.second << "; ";
        }
    }
    catch(std::ifstream::failure e){
        std::cerr << "Exception opening/reading/closing file\n";
    }

    return 0;
}