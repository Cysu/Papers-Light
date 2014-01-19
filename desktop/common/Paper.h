#ifndef PAPER_H
#define PAPER_H

#include <string>
#include <vector>

class Paper
{
public:
    Paper();

    /**
     * @brief Paper contains basic information of a paper
     * @param id The id of the paper in the database
     * @param year The year when the paper is published
     * @param bookTitle The conference, journal or book where the paper is published
     * @param title The title of the paper
     * @param authors The authors list of the paper
     * @param tags The tags list of the paper
     * @param path The path of the paper relative to the paper home folder
     * @param comment The comment of the paper
     */
    Paper(int id,
          int year,
          const std::string& bookTitle,
          const std::string& title,
          const std::vector<std::string>& authors,
          const std::vector<std::string>& tags,
          const std::string& path,
          const std::string& comment);

    ~Paper();

    bool isNull() const;

    int getId() const { return id_; }
    void setId(int id) { id_ = id; }

    int getYear() const { return year_; }
    void setYear(int year) { year_ = year; }

    const std::string& getBookTitle() const { return bookTitle_; }
    void setBookTitle(const std::string& bookTitle) { bookTitle_ = bookTitle; }

    const std::string& getTitle() const { return title_; }
    void setTitle(const std::string& title) { title_ = title; }

    const std::vector<std::string>& getAuthors() const { return authors_; }
    void setAuthors(const std::vector<std::string>& authors) { authors_ = authors; }

    const std::vector<std::string>& getTags() const { return tags_; }
    void setTags(const std::vector<std::string>& tags) { tags_ = tags; }

    const std::string& getPath() const { return path_; }
    void setPath(const std::string& path) { path_ = path; }

    const std::string& getComment() const { return comment_; }
    void setComment(const std::string& comment) { comment_ = comment; }

private:
    int id_;
    int year_;
    std::string bookTitle_;
    std::string title_;
    std::vector<std::string> authors_;
    std::vector<std::string> tags_;
    std::string path_;
    std::string comment_;
};

#endif // PAPER_H
