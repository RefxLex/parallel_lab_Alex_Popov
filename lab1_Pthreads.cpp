#include <iostream>
#include <pthread.h>
#include <fstream>
#include <map>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <semaphore.h>

using namespace std;

string file_name="text1.txt";
map <string, int> words_container;
vector<string> words_array;
sem_t container_sem;

void *searchWord(void *arg){

    string *buff = (string*) arg;
    string word = *buff;
    ifstream readFile(file_name);
    string textLine;
    int count;
    while (getline(readFile,textLine)) {
            
        int word_lenght = word.length();
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

    // заблокируем семафор (декремент) если его значение уже не равно 0 (семафор занят)
    sem_wait(&container_sem);

    //теперь можно выполнять действия
    words_container[word]=count;

    //освободим семафор
    sem_post(&container_sem);

    pthread_exit(0);

}

int main (){

        cout<<"Введите имя файла \n";
        cin>>file_name;
        int N;
        string searched_word;

        cout<<"Введите число слов \n";
        cin>>N;
        cout<<"Ввод искомых слов \n";
        for(int i=0; i<N; i++){
          cin>>searched_word;
          words_container.insert({{searched_word,0}});
          words_array.push_back(searched_word);
        }

        // замерим время выполнения
        auto start = std::chrono::system_clock::now();
        std::chrono::duration<double> diff;

        // для измерения запустим 10.тыс раз
        for(int j=0; j<10000; j++){
         
            /*
            инициализируем семафор значениями (&sem, 0, 1)
            0 - семафор общий для всех потоков процесса
            при начальном значении 1 семафор работает как мьютекс
            */
            sem_init(&container_sem, 0, 1);
            pthread_t thid[N];

            for (int i=0; i<N; i++) {

                if(pthread_create(&thid[i],NULL,searchWord,&words_array[i]) !=0 ) {
                    perror("pthread_create() error");
                    exit(1);
                }
            }

            for (int e=0; e<N; e++){
                pthread_join(thid[e], NULL);
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