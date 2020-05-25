# rice::vector

## The vector container that you truly deserve as the best programmer in the world! :sunglasses: :metal:

## *rice::vector* function examples:

### vector(vector<T>::size_type, const T)
Constructor, that creates a vector from given size, and fills it with the given value, if specified. Otherwise a default type value is used.
```shell
template <typename T>
vector<T>::vector(vector<T>::size_type n, const T &value = T())
{
    grow_allocate(n);
    std::uninitialized_fill(buffer, buffer + n, value);
    vec_sz = n;
}
```

### vector<T>::insert(const_iterator pos, const T &val = T())
Inserts a value specified (if specified), into the position given.
```shell
template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const T &val = T())
{
    size_type index = pos - buffer;
    if (vec_sz == cap_sz)
        grow_reallocate(cap_sz);

    iterator it = &buffer[index];
    std::uninitialized_move(it, buffer + vec_sz, it + 1);
    (*it) = val;
    ++vec_sz;

    return it;
}
```
### vector<T>::erase(const_iterator first, const_iterator last)
Function that erases vector elements from first iterator to last, last is not erased.
```shell
template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
{
    iterator f = &buffer[first - buffer], l = &buffer[last - buffer], d = f;
    if (first == last)
        return f;
    for (; d != l; ++d)
        allocator.destroy(d);
    std::uninitialized_move(l, buffer + vec_sz, f);

    vec_sz -= last - first;
    return f;
}
```
### vector<T>::clear()
Function that clears elements of a vector. The size is set to 0, however the capacity remains unchanged.
```shell
template <typename T>
void vector<T>::clear()
{
    for (size_type i = 0; i < vec_sz; ++i)
        allocator.destroy(buffer + i);
    vec_sz = 0;
}
```
### vector<T>::swap(vector<T> &rhs)
Function that swaps the elements of two vectors.
```shell
template <typename T>
void vector<T>::swap(vector<T> &rhs)
{
    size_t temp_vec_sz = vec_sz,
        temp_cap_sz = cap_sz;
    T *temp_buffer = buffer;

    vec_sz = rhs.vec_sz;
    vec_sz = rhs.vec_sz;
    buffer = rhs.buffer;

    rhs.vec_sz = temp_vec_sz;
    rhs.vec_sz = temp_cap_sz;
    rhs.buffer = temp_buffer;
}
```
## Testing

### PC Specs:
| CPU                 | RAM  |
| ------------------- | ---- |
| Intel Core i7 8086k | 16GB |

## Speed testing/comparing *rice::vector* speed to *std::vector*

Tests were done with the *-O3* flag and without. 
*rice::vector* was comparetively alot faster than *std::vector* in all scenarios.

| Size         | std::vector | rice::vector |
| ------------ | ----------- | ------------ |
| 10 000       | 0.0001867   | 0.0001925    |
| 100 000      | 0.001818    | 0.0013717    |
| 1 000 000    | 0.0168036   | 0.0108323    |
| 10 000 000   | 0.18722     | 0.120501     |
| 100 000 000  | 1.69484     | 1.03986      |
| 1000 000 000 | 16.0132     | 11.1249      |
 
 with -O3
| Size         | std::vector | rice::vector |
| ------------ | ----------- | ------------ |
| 10 000       | 0.0001056   | 8.14e-05     |
| 100 000      | 0.0009006   | 0.0008856    |
| 1 000 000    | 0.0082982   | 0.0060223    |
| 10 000 000   | 0.108814    | 0.0711907    |
| 100 000 000  | 0.894719    | 0.53218      |
| 1000 000 000 | 7.96962     | 5.99519      |

### Conclusions:
- *std::vector* is slower than *rice::vector*
- Using *rice::vector* instead of *std::vector* is recommended.
- Optimization flags further improve the speed of the containers.

## Reallocation count comaparison 

Reallocations
* *std::vector* - 25 times.
* *rice::vector* - 11 times.

### Conclusions:
- Reallocation count is by far smaller on rice::vector comparted to *std::vector*.

##  Speed comparson in [students-copy v2.2](https://github.com/NeonRice/students-copy)
The following tests were done with the optimization flag on.

**Reading 10 000 000 entries**
* *std::vector* - 15.7603 s
* *rice::vector* - 17.3734 s

**Splitting 10 000 000 entries**
* std::vector -  1.67902 s
* *rice::vector* -  1.71362 s


### Conclusions:
- *rice::vector* completes the tests faster.
- *rice::vector* is preferred for the program.

## Using instructions for rice::vector
1. Download *vector.h* with your preffered method.
2. Put the *vector.h* into your projects directory.
3. Include it how you would include any other non-library header and use it like you would use a std::vector
Example: *rice::vector<int> numbers = {1, 2, 3, 4, 5};*
