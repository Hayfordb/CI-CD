  
## Part 1. Настройка и запуск       

##### Подними виртуальную машину *Ubuntu Server 22.04 LTS*.

![alt text](screen/1.1.png)

##### Скачай и установи на виртуальную машину **gitlab-runner**. (для удобства ввода команд далее использую WSL через ssh к поднятому хосту)

Добавляем официальный репу с гитлаб раннером и скачиваем актуальную версию

![alt text](screen/1.2.png)


![alt text](screen/1.3.png)

##### Запусти **gitlab-runner** и зарегистрируй его для использования в текущем проекте (*DO6_CICD*).
- Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе.

![alt text](screen/1.4.png)

## Part 2. Сборка

### Напишем этап для **CI** по сборке приложений из проекта *C2_SimpleBashUtils*.

##### создадим _gitlab-ci.yml_ с этапом запуска сборки через мейк файл из проекта _C2_.

##### Файлы, полученные после сборки (артефакты), сохрани в произвольную директорию со сроком хранения 30 дней.

![alt text](screen/1.5.png)

- stages определяет последовательность этапов выполнения.
- build - это название задачи (job) для этапа сборки.
- stage: build - указывает, что этот job относится к этапу build.
- script содержит команды, которые GitLab Runner будет выполнять. Здесь вызывается команда `make`, которая будет запускать процесс сборки проекта с помощью Makefile.
- artifacts определяет, какие файлы или папки будут сохранены после выполнения этой задачи. В приведенном примере src/artifacts - это путь к собранным артефактам, которые нужно сохранить. При этом команда cd не требуется.
- expire_in - устанавливает скрок хранения артефактов.

## Part 3. Тест кодстайла


#### Напишем этап для **CI**, который запускает скрипт кодстайла (*clang-format*).

##### Если кодстайл не прошел, то «зафейлим» пайплайн.

![alt text](screen/1.6.png)

##### В пайплайне отобразим вывод утилиты *clang-format*.

![alt text](screen/1.8.png)  

![alt text](screen/1.7.png)


### Part 4. Интеграционные тесты

#### Напишем этап для **CI**, который запускает твои интеграционные тесты из того же проекта.

##### Запусти этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно.    


<pre>
```stages:  
  - build
  - code_style
  - integration_tests

build_cat_and_grep:
  stage: build
  script:
    - make -C src/cat
    - make -C src/grep
  artifacts:
    paths:
      - src/cat/s21_cat
      - src/grep/s21_grep
    expire_in: 30 days 
    when: always

code_style_check:
  stage: code_style
  before_script:
    - cp materials/linters/.clang-format src/cat
    - cp materials/linters/.clang-format src/grep
  script:
    - cd src/cat
    - clang-format -n --Werror *.c
    - cd ../grep
    - clang-format -n --Werror *.c
  allow_failure: false

test_cat:
  stage: integration_tests
  script:
    - cd src/cat
    - chmod +x tests.sh
    - ./tests.sh > test_out_cat.txt || (cat test_out_cat.txt && exit 1)
  artifacts:
    paths:
      - src/cat
    expire_in: 30 days 
    when: always
  dependencies:
    - build_cat_and_grep
    - code_style_check
  allow_failure: false

test_grep:
  stage: integration_tests
  script:
    - cd src/grep
    - chmod +x tests.sh
    - ./tests.sh > test_out_grep.txt || (cat test_out_grep.txt && exit 1)
  artifacts:
    paths:
      - src/grep
    expire_in: 30 days 
    when: always
  dependencies:
    - build_cat_and_grep
    - code_style_check
  allow_failure: false
```
</pre>

- Добавляю новый этап и новую задачу integration_tests
- Перехожу в папки с проектами и запускю тесты
- Выдаю права на выполнение
- Передаю весь вывод в *outut.txt файлы и вывожу на "экран" этот файл с помощью cat
- dependencies - этот ключ позволяет указать зависимости данного этапа от успешного завершения других этапов.
- allow_failure: false - указывает GitLab не разрешать "успешное" выполнение пайплайна в случае ошибки в этом этапе.

### Part 5. Этап деплоя

 ##### Поднял 2-ую тачку для реализации на ней стадии CD

 ![alt text](screen/2.0.png)

 #### Дописал этап для **CD**, который «доставляет» проект на другую тачку.

![alt text](screen/2.1.png)


##### Напиcал bash-скрипт, который при помощи **ssh** и **scp** копирует файлы, полученные после сборки (артефакты), в директорию */usr/local/bin* второй виртуальной машины.

``` #!/bin/bash
  
scp ./src/cat/s21_cat root@192.168.31.187:/usr/local/bin 
scp ./src/grep/s21_grep root@192.168.31.187:/usr/local/bin  

if [ $? -eq 0 ]; then
    echo "Deploy files passed!"
else
    echo "Deploy files failed!"
    exit 1  
fi
```
![alt text](screen/2.2.png)

![alt text](screen/2.3.png)

### Part 6. Дополнительно. Уведомления в Телегу


##### Настроил уведомления о успешном/неуспешном выполнении пайплайна через бота в *Telegram*.

- написал bash скрипт notify.sh c данными бота:

  ``` #!/bin/bash
  TELEGRAM_TOKEN=6512534775:AAEglfN6IOvME77NQ5S46ksfnjd09maPfBg
  TELEGRAM_QUARKRON=70735394
  TIME=10 

  URL="https://api.telegram.org/bot7092707704:AAFYQprtZbxEFeEVknO4Yswmpf4AwKjzLTw/sendMessage"
  TEXT="Deploy status: $1%0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG" 

  curl -s --max-time $TIME -d "chat_id=1139317201&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null
  ```
- Добавил пункт after_script в .gitlab-ci.yml в каждую джобу

  ```
  after_script:
      - ./notify.sh "test_cat"
  ```
- запустил и протестил

![alt text](screen/2.4.png)
