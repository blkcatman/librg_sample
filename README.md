## librg_sample

### 必要環境

- docker

### アプリのビルド方法

以下のコマンドを実行する
```
docker-compose build
docker-comopse run --rm --entrypoint "./build.sh" app
```

### アプリの実行方法

以下のコマンドを実行する
```
docker-comopse run --rm --entrypoint "./run.sh" app
```
