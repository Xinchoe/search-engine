#ifndef INCLUDE_NONCOPYABLE_H_
#define INCLUDE_NONCOPYABLE_H_

class Noncopyable {
 protected:
  Noncopyable() = default;
  ~Noncopyable() = default;

 private:
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;
};

#endif
