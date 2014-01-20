<?php

$type2attr = [
    'article' => [
        'required' => ['author', 'title', 'journal', 'year'],
        'optional' => ['volume', 'number', 'pages', 'month', 'note']
    ],
    'book' => [
        'required' => ['author', 'title', 'publisher', 'year'],
        'optional' => ['volumn', 'series', 'address', 'edition', 'month', 'note']
    ],
    'booklet' => [
        'required' => ['title'],
        'optional' => ['author', 'howpublished', 'address', 'month', 'year', 'note']
    ],
    'conference' => [
        'required' => ['author', 'title', 'booktitle', 'year'],
        'optional' => ['editor', 'volume', 'series', 'pages', 'address', 'month', 'organization', 'publisher', 'note']
    ],
    'inbook' => [
        'required' => ['author', 'title', 'pages', 'publisher', 'year'],
        'optional' => ['volume', 'series', 'type', 'address', 'edition', 'month', 'note']
    ],
    'incollection' => [
        'required' => ['author', 'title', 'booktitle', 'publisher', 'year'],
        'optional' => [ 'editor', 'volume', 'series', 'type', 'chapter', 'pages', 'address', 'edition', 'month', 'note']
    ],
    'inproceedings' => [
        'required' => ['author', 'title', 'booktitle', 'year'],
        'optional' => ['editor', 'volume', 'series', 'pages', 'address', 'month', 'organization', 'publisher', 'note']
    ],
    'manual' => [
        'required' => ['title'],
        'optional' => ['author', 'organization', 'address', 'editor', 'month', 'year', 'note']
    ],
    'mastersthesis' => [
        'required' => ['author', 'title', 'school', 'year'],
        'optional' => ['type', 'address', 'month', 'note']
    ],
    'misc' => [
        'required' => [],
        'optional' => ['author', 'title', 'howpublished', 'month', 'year', 'note']
    ],
    'phdthesis' => [
        'required' => ['author', 'title', 'school', 'year'],
        'optional' => ['type', 'address', 'month', 'note']
    ],
    'proceedings' => [
        'required' => ['title', 'year'],
        'optional' => ['editor', 'volume', 'series', 'address', 'month', 'publisher', 'organization', 'note']
    ],
    'techreport' => [
        'required' => ['author', 'title', 'institution', 'year'],
        'optional' => ['type', 'number', 'address', 'month', 'note']
    ],
    'unpublished' => [
        'required' => ['author', 'title', 'note'],
        'optional' => ['month', 'year']
    ]
];
