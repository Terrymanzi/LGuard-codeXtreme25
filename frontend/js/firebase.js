const fetchAccidents = async () => {
    const response = await fetch("http://localhost:5000/api/accidents");
    const data = await response.json();
    return data;
  };
  