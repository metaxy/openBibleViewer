cd ../src
lconvert -i obv_en.ts -o obv_en.po --of po
lconvert -i obv_de.ts -o obv_de.po --of po
lconvert -i obv_ru.ts -o obv_ru.po --of po
lconvert -i obv_cs.ts -o obv_cs.po --of po

tar -cvf po.tar obv_en.po obv_de.po obv_ru.po obv_cs.po

