<?php

$type2attr = [
    "article" => [
        "required" => ["author", "title", "journal", "year"],
        "optional" => ["volume", "number", "pages", "month", "note"]
    ],
    "book" => [
        "required" => ["author", "title", "publisher", "year"],
        "optional" => ["volumn", "series", "address", "edition", "month", "note"]
    ],
    "booklet" => [
        "required" => ["title"],
        "optional" => ["author", "howpublished", "address", "month", "year", "note"]
    ],
    "conference" => [
        "required" => ["author", "title", "booktitle", "year"],
        "optional" => ["editor", "volume", "series", "pages", "address", "month", "organization", "publisher", "note"]
    ],
    "inbook" => [
        "required" => ["author", "title", "pages", "publisher", "year"],
        "optional" => ["volume", "series", "type", "address", "edition", "month", "note"]
    ],
    "incollection" => [
        "required" => ["author", "title", "booktitle", "publisher", "year"],
        "optional" => [ "editor", "volume", "series", "type", "chapter", "pages", "address", "edition", "month", "note"]
    ],
    "inproceedings" => [
        "required" => ["author", "title", "booktitle", "year"],
        "optional" => ["editor", "volume", "series", "pages", "address", "month", "organization", "publisher", "note"]
    ],
    "manual" => [
        "required" => ["title"],
        "optional" => ["author", "organization", "address", "editor", "month", "year", "note"]
    ],
    "mastersthesis" => [
        "required" => ["author", "title", "school", "year"],
        "optional" => ["type", "address", "month", "note"]
    ],
    "misc" => [
        "required" => [],
        "optional" => ["author", "title", "howpublished", "month", "year", "note"]
    ],
    "phdthesis" => [
        "required" => ["author", "title", "school", "year"],
        "optional" => ["type", "address", "month", "note"]
    ],
    "proceedings" => [
        "required" => ["title", "year"],
        "optional" => ["editor", "volume", "series", "address", "month", "publisher", "organization", "note"]
    ],
    "techreport" => [
        "required" => ["author", "title", "institution", "year"],
        "optional" => ["type", "number", "address", "month", "note"]
    ],
    "unpublished" => [
        "required" => ["author", "title", "note"],
        "optional" => ["month", "year"]
    ]
];

echo <<<END
CREATE DATABASE IF NOT EXISTS papers_light DEFAULT CHARSET UTF8 COLLATE UTF8_GENERAL_CI;
USE papers_light;
CREATE USER "pl_admin" IDENTIFIED BY "cuhk_mmlab";
GRANT ALL ON papers_light.* TO "pl_admin";

END;

foreach ($type2attr as $type => $attr) {
    echo "CREATE TABLE IF NOT EXISTS pl_{$type}(\n";
    echo "    id INT AUTO_INCREMENT PRIMARY KEY,\n";
    $cols = [];
    foreach ($attr["required"] as $a) {
        array_push($cols, "    {$a} TEXT NOT NULL");
    }
    foreach ($attr["optional"] as $a) {
        array_push($cols, "    {$a} TEXT");
    }
    echo join(",\n", $cols);
    echo "\n);\n";
}
