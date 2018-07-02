for file in ins/*
do
    echo "criando rótulos para $file"
    ./gerador $file
done

echo "Fim"
